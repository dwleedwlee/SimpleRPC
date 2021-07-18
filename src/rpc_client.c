#include <stdio.h>
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"

static t_rpc_client_info g_rpcClientInfo = {
	.stat = RPC_CLIENT_READY,
	.fp = NULLPTR
};

void rpcRunClientObserver(void) {
	
	switch (g_rpcClientInfo.stat) {
		
		case RPC_CLIENT_READY:
			break;
			
		case RPC_CLIENT_REQUEST:
		{
			const t_rpc_server_stat srvStat = rpcServerStatRead();
			
			switch(srvStat) {
				case RPC_SERVER_ACCEPT:
					/* Checking Timeout */ 
					break;
				case RPC_SERVER_UNDEFINED:
				case RPC_SERVER_ERROR:
				case RPC_SERVER_FINISH:
				{
					t_fp_callback fp = g_rpcClientInfo.fp;
					rpcClientStatWrite(RPC_CLIENT_READY);
					g_rpcClientInfo.stat = RPC_CLIENT_READY;
					if(fp != NULLPTR) {
						fp();
					}
				}
					break;
			}
		}			
			break;
		
		default:
			break;
	}
}

t_rpc_req_ret rpcRequestService(t_fp_callback fp) {
	t_rpc_req_ret ret;
	
	if(g_rpcClientInfo.stat == RPC_CLIENT_READY) {
		rpcClientStatWrite(RPC_CLIENT_REQUEST);
		g_rpcClientInfo.stat = RPC_CLIENT_REQUEST;
		g_rpcClientInfo.fp = fp;
		ret = RPC_REQUEST_OK;
	} else {
		ret = RPC_REQUEST_ERR;
	}
	return ret;
}
