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
						rpcServerStatWrite(i, RPC_SERVER_ACCEPT);
						g_rpcServerInfo[i].stat = RPC_SERVER_ACCEPT;
						g_rpcServerInfo[i].processStat = RPC_PROCESS_PENDING;
						fp(i, buf); /* Blocking Call */
					} else {
						rpcServerStatWrite(i, RPC_SERVER_UNDEFINED);
						g_rpcServerInfo[i].stat = RPC_SERVER_UNDEFINED;
					}				
				}
			}
				break;
				
			case RPC_SERVER_ACCEPT:
			{
				t_rpc_process_stat processStat = g_rpcServerInfo[i].processStat;
				if(processStat != RPC_PROCESS_PENDING) {
					if(processStat == RPC_PROCESS_OK) {
						rpcServerStatWrite(i, RPC_SERVER_FINISH);
						g_rpcServerInfo[i].stat = RPC_SERVER_FINISH;
					} else {
						rpcServerStatWrite(i, RPC_SERVER_ERROR);
						g_rpcServerInfo[i].stat = RPC_SERVER_ERROR;
					}
					g_rpcServerInfo[i].processStat = RPC_PROCESS_REDAY;
				}
			}			
				break;
			
			case RPC_SERVER_UNDEFINED:
			case RPC_SERVER_ERROR:
			case RPC_SERVER_FINISH:
			{
				const t_rpc_client_stat cliStat = rpcClientStatRead(i);
				if(cliStat == RPC_CLIENT_READY) {
					rpcServerStatWrite(i, RPC_SERVER_READY);
					g_rpcServerInfo[i].stat = RPC_SERVER_READY;
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
