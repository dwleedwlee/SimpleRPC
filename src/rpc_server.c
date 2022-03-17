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

#include "rpc_server.h"
#include "rpc_buffer.h"

static t_rpc_server_info g_rpcServerInfo[RPC_GENERAL_ITEM_MAX];
 
void rpcInitServerInfo(void) {
	t_rpc_item i;
	for(i = RPC_GENERAL_ITEM_START; i < RPC_GENERAL_ITEM_MAX; i++) {
		g_rpcServerInfo[i].stat = RPC_CLIENT_READY;
		g_rpcServerInfo[i].processStat = RPC_PROCESS_REDAY;
		g_rpcServerInfo[i].fp = NULLPTR;
	}
}

void rpcRunServerObserver(void) {
	t_rpc_item i;
	for(i = RPC_GENERAL_ITEM_START; i < RPC_GENERAL_ITEM_MAX; i++) {
		
		switch (g_rpcServerInfo[i].stat) {
			
			case RPC_SERVER_READY:
			{
				const t_rpc_client_stat cliStat = rpcClientStatRead(i);
				if(cliStat == RPC_CLIENT_REQUEST) {
					t_fp_srv_callback fp = g_rpcServerInfo[i].fp;
					if(fp != NULLPTR) {
						uint8 buf[RPC_DATA_BUF_SIZE];
						rpcReqDataRead(i, buf);
						
						g_rpcServerInfo[i].stat = RPC_SERVER_ACCEPT;
						g_rpcServerInfo[i].processStat = RPC_PROCESS_PENDING;
						
						rpcServerStatWrite(i, RPC_SERVER_ACCEPT);
						fp(i, buf); /* Blocking Call */
					} else {
						g_rpcServerInfo[i].stat = RPC_SERVER_REJECT;
						rpcServerStatWrite(i, RPC_SERVER_REJECT);						
					}				
				}
			}
				break;
				
			case RPC_SERVER_ACCEPT:
			{
				t_rpc_process_stat processStat = g_rpcServerInfo[i].processStat;
				if(processStat != RPC_PROCESS_PENDING) {
					if(processStat == RPC_PROCESS_OK) {						
						g_rpcServerInfo[i].stat = RPC_SERVER_FINISH;
						rpcServerStatWrite(i, RPC_SERVER_FINISH);
					} else {						
						g_rpcServerInfo[i].stat = RPC_SERVER_ERROR;
						rpcServerStatWrite(i, RPC_SERVER_ERROR);
					}
					g_rpcServerInfo[i].processStat = RPC_PROCESS_REDAY;
				}
			}			
				break;
			
			case RPC_SERVER_REJECT:
			case RPC_SERVER_ERROR:
			case RPC_SERVER_FINISH:
			{
				const t_rpc_client_stat cliStat = rpcClientStatRead(i);
				if(cliStat == RPC_CLIENT_READY) {					
					g_rpcServerInfo[i].stat = RPC_SERVER_READY;
					rpcServerStatWrite(i, RPC_SERVER_READY);
				}
			}
				break;
				
			default:
				break;
		}
		
	}
}

void rpcRegisterService(t_rpc_item item, t_fp_srv_callback fp) {
	g_rpcServerInfo[item].fp = fp;
}

void rpcSetProcessStat(t_rpc_item item, t_rpc_process_stat stat) {
	g_rpcServerInfo[item].processStat = stat;
}
