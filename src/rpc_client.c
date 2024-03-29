/*
	The MIT License (MIT)
	
	Copyright (c) 2021 Dongwook Lee
	
	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <windows.h>

#include "rpc_client.h"
#include "rpc_buffer.h"
#include "rpc_util.h"

static t_rpc_client_info g_rpcClientInfo[RPC_GENERAL_ITEM_MAX];

static CRITICAL_SECTION CriticalSection;

#ifdef FEATURE_DUAL_BUFFER
static t_dual_buf_info gDualBufInfo[RPC_GENERAL_ITEM_MAX];
#endif

void rpcInitClientInfo(void) {
	InitializeCriticalSection(&CriticalSection);
	
	t_rpc_item i;
	for(i = RPC_GENERAL_ITEM_START; i < RPC_GENERAL_ITEM_MAX; i++) {
		g_rpcClientInfo[i].stat = RPC_CLIENT_READY;
		g_rpcClientInfo[i].fp = NULLPTR;
		
		/* linked array */
		g_rpcClientInfo[i].prev = RPC_GENERAL_ITEM_START;
		g_rpcClientInfo[i].next = RPC_GENERAL_ITEM_START;
		
		#ifdef FEATURE_DUAL_BUFFER
		gDualBufInfo[i].active_buf_idx = FALSE;
		gDualBufInfo[i].stat = DUAL_BUF_STAT_EMPTY;
		#endif
	}
}

static void linkedArrayInsertItem(t_rpc_item item) {
	/* linked array - insert the item next to the head */
	t_rpc_item next = g_rpcClientInfo[RPC_GENERAL_ITEM_START].next;
	g_rpcClientInfo[RPC_GENERAL_ITEM_START].next = item;
	g_rpcClientInfo[item].prev = RPC_GENERAL_ITEM_START;
	g_rpcClientInfo[item].next = next;
	g_rpcClientInfo[next].prev = item;
	
}

static void linkedArrayDeleteItem(t_rpc_item item) {
	/* linked array - delete the item */			
	t_rpc_item prev = g_rpcClientInfo[item].prev;
	t_rpc_item next = g_rpcClientInfo[item].next;
	g_rpcClientInfo[prev].next = g_rpcClientInfo[item].next;
	g_rpcClientInfo[next].prev = g_rpcClientInfo[item].prev;
}

void rpcRunClientObserver(void) {
	t_rpc_item i;
	
	for(i = g_rpcClientInfo[RPC_GENERAL_ITEM_START].next; i != RPC_GENERAL_ITEM_START; i = g_rpcClientInfo[i].next) {
		switch (g_rpcClientInfo[i].stat) {
			
			case RPC_CLIENT_READY:
			#ifdef FEATURE_DUAL_BUFFER
			
				EnterCriticalSection(&CriticalSection);
				
				if(gDualBufInfo[i].stat != DUAL_BUF_STAT_EMPTY) {
					const t_rpc_server_stat srvStat = rpcServerStatRead(i);
					if(srvStat == RPC_SERVER_READY) {
						uint8 idx = gDualBufInfo[i].active_buf_idx;
						rpcReqDataWrite(i, gDualBufInfo[i].rpc_buf[idx].buf, gDualBufInfo[i].rpc_buf[idx].size);
						
						g_rpcClientInfo[i].fp = gDualBufInfo[i].fp[idx];
						g_rpcClientInfo[i].stat = RPC_CLIENT_REQUEST;
						
						rpcClientStatWrite(i, RPC_CLIENT_REQUEST);
					
						if(gDualBufInfo[i].stat == DUAL_BUF_STAT_SINGLE) {
							linkedArrayDeleteItem(i);
						
							gDualBufInfo[i].stat = DUAL_BUF_STAT_EMPTY;
						} else { /* DUAL_BUF_STAT_FULL */
							gDualBufInfo[i].active_buf_idx = (gDualBufInfo[i].active_buf_idx + 1) % 2;
							gDualBufInfo[i].stat = DUAL_BUF_STAT_SINGLE;
						}						
					}
				}
				
				LeaveCriticalSection(&CriticalSection);
				
			#endif
				break;
				
			case RPC_CLIENT_REQUEST:
			{
				const t_rpc_server_stat srvStat = rpcServerStatRead(i);
				
				switch(srvStat) {
					case RPC_SERVER_ACCEPT:
						/* Checking Timeout */ 
						break;
					case RPC_SERVER_REJECT:
					case RPC_SERVER_ERROR:
					case RPC_SERVER_FINISH:
					{
						t_fp_cli_callback fp = g_rpcClientInfo[i].fp;						
						if(fp != NULLPTR) {
							fp(i); /* Blocking Call */
						}
					#ifndef FEATURE_DUAL_BUFFER
						linkedArrayDeleteItem(i);						
					#endif
						g_rpcClientInfo[i].stat = RPC_CLIENT_READY;
						rpcClientStatWrite(i, RPC_CLIENT_READY);
					}
						break;
				}
			}			
				break;
			
			default:
				break;
		}
	
	}
}

#ifdef FEATURE_DUAL_BUFFER
t_rpc_req_ret rpcRequestService(t_rpc_item item, t_fp_cli_callback fp, t_rpc_buf *rpc_buf) {
	
	EnterCriticalSection(&CriticalSection);
	
	switch(gDualBufInfo[item].stat) {
		uint8 idx;
		case DUAL_BUF_STAT_EMPTY:
			gDualBufInfo[item].stat = DUAL_BUF_STAT_SINGLE;
			
			idx = gDualBufInfo[item].active_buf_idx;
			gDualBufInfo[item].fp[idx] = fp;
			gDualBufInfo[item].rpc_buf[idx] = *rpc_buf;	
			
			linkedArrayInsertItem(item);			
			
			break;
		case DUAL_BUF_STAT_SINGLE:
			gDualBufInfo[item].stat = DUAL_BUF_STAT_FULL;
			
			idx = (gDualBufInfo[item].active_buf_idx + 1) % 2;
			gDualBufInfo[item].fp[idx] = fp;
			gDualBufInfo[item].rpc_buf[idx] = *rpc_buf;			
			break;
		case DUAL_BUF_STAT_FULL: /* overwrite */
			idx = (gDualBufInfo[item].active_buf_idx + 1) % 2;
			gDualBufInfo[item].fp[idx] = fp;
			gDualBufInfo[item].rpc_buf[idx] = *rpc_buf;
			break;
		default:
			break;
	}
	
	LeaveCriticalSection(&CriticalSection);
	
	return RPC_REQUEST_OK;
}
#else
t_rpc_req_ret rpcRequestService(t_rpc_item item, t_fp_cli_callback fp, t_rpc_buf *rpc_buf) {
	t_rpc_req_ret ret;
	
	EnterCriticalSection(&CriticalSection);
	
	if(g_rpcClientInfo[item].stat == RPC_CLIENT_READY) {
		const t_rpc_server_stat srvStat = rpcServerStatRead(item);
		if(srvStat == RPC_SERVER_READY) {
			rpcReqDataWrite(item, rpc_buf->buf, rpc_buf->size);
			
			g_rpcClientInfo[item].stat = RPC_CLIENT_REQUEST;
			g_rpcClientInfo[item].fp = fp;
			
			rpcClientStatWrite(item, RPC_CLIENT_REQUEST);
			
			linkedArrayInsertItem(item);
			
			ret = RPC_REQUEST_OK;
		} else {
			ret = RPC_REQUEST_ERR;
		}
	} else {
		ret = RPC_REQUEST_ERR;
	}
	
	LeaveCriticalSection(&CriticalSection);
	
	return ret;
}
#endif
