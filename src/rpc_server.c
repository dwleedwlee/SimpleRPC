#include <stdio.h>
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"

static t_rpc_server_info g_rpcServerInfo = {
	.stat = RPC_SERVER_READY,
	.processStat = RPC_PROCESS_REDAY,
	.fp = NULLPTR
};

void rpcRunServerObserver(void) {
	
	switch (g_rpcServerInfo.stat) {
		
		case RPC_SERVER_READY:
		{
			const t_rpc_client_stat cliStat = rpcClientStatRead();
			if(cliStat == RPC_CLIENT_REQUEST) {
				t_fp_callback fp = g_rpcServerInfo.fp;
				if(fp != NULLPTR) {
					rpcServerStatWrite(RPC_SERVER_ACCEPT);
					g_rpcServerInfo.stat = RPC_SERVER_ACCEPT;
					g_rpcServerInfo.processStat = RPC_PROCESS_PENDING;
					fp();
				} else {
					rpcServerStatWrite(RPC_SERVER_UNDEFINED);
					g_rpcServerInfo.stat = RPC_SERVER_UNDEFINED;
				}				
			}
		}
			break;
			
		case RPC_SERVER_ACCEPT:
		{
			t_rpc_process_stat processStat = g_rpcServerInfo.processStat;
			if(processStat != RPC_PROCESS_PENDING) {
				if(processStat == RPC_PROCESS_OK) {
					rpcServerStatWrite(RPC_SERVER_FINISH);
					g_rpcServerInfo.stat = RPC_SERVER_FINISH;
				} else {
					rpcServerStatWrite(RPC_SERVER_ERROR);
					g_rpcServerInfo.stat = RPC_SERVER_ERROR;
				}
				g_rpcServerInfo.processStat = RPC_PROCESS_REDAY;
			}
		}			
			break;
		
		case RPC_SERVER_UNDEFINED:
		case RPC_SERVER_ERROR:
		case RPC_SERVER_FINISH:
		{
			const t_rpc_client_stat cliStat = rpcClientStatRead();
			if(cliStat == RPC_CLIENT_READY) {
				rpcServerStatWrite(RPC_SERVER_READY);
				g_rpcServerInfo.stat = RPC_SERVER_READY;
			}
		}
			break;
			
		default:
			break;
	}
}

void rpcRegisterService(t_fp_callback fp) {
	g_rpcServerInfo.fp = fp;
}

void rpcSetProcessStat(t_rpc_process_stat stat) {
	g_rpcServerInfo.processStat = stat;
}
