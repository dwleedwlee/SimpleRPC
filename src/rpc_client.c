#include <stdio.h>
#include <windows.h>

#include "rpc_client.h"
#include "rpc_buffer.h"

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
		#ifdef FEATURE_DUAL_BUFFER
		gDualBufInfo[i].active_buf_idx = FALSE;
		gDualBufInfo[i].stat = DUAL_BUF_STAT_EMPTY;
		#endif
	}
}

void rpcRunClientObserver(void) {
	t_rpc_item i;
	for(i = RPC_GENERAL_ITEM_START; i < RPC_GENERAL_ITEM_MAX; i++) {
		
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
					case RPC_SERVER_UNDEFINED:
					case RPC_SERVER_ERROR:
					case RPC_SERVER_FINISH:
					{
						t_fp_cli_callback fp = g_rpcClientInfo[i].fp;						
						if(fp != NULLPTR) {
							fp(i); /* Blocking Call */
						}						
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
	
	if(g_rpcClientInfo[item].stat == RPC_CLIENT_READY) {
		const t_rpc_server_stat srvStat = rpcServerStatRead(item);
		if(srvStat == RPC_SERVER_READY) {
			rpcReqDataWrite(item, rpc_buf->buf, rpc_buf->size);
			
			g_rpcClientInfo[item].stat = RPC_CLIENT_REQUEST;
			g_rpcClientInfo[item].fp = fp;
			
			rpcClientStatWrite(item, RPC_CLIENT_REQUEST);
			
			ret = RPC_REQUEST_OK;
		} else {
			ret = RPC_REQUEST_ERR;
		}
	} else {
		ret = RPC_REQUEST_ERR;
	}
	return ret;
}
#endif
