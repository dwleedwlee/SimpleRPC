#include <stdio.h>

#include "rpc_client.h"
#include "rpc_buffer.h"

static t_rpc_client_info g_rpcClientInfo[RPC_GENERAL_ITEM_MAX];

void rpcInitClientInfo(void) {
	t_rpc_item i;
	for(i = RPC_GENERAL_ITEM_START; i < RPC_GENERAL_ITEM_MAX; i++) {
		g_rpcClientInfo[i].stat = RPC_CLIENT_READY;
		g_rpcClientInfo[i].fp = NULLPTR;
	}
}

void rpcRunClientObserver(void) {
	t_rpc_item i;
	for(i = RPC_GENERAL_ITEM_START; i < RPC_GENERAL_ITEM_MAX; i++) {
		
		switch (g_rpcClientInfo[i].stat) {
			
			case RPC_CLIENT_READY:
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
						rpcClientStatWrite(i, RPC_CLIENT_READY);
						g_rpcClientInfo[i].stat = RPC_CLIENT_READY;
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

t_rpc_req_ret rpcRequestService(t_rpc_item item, t_fp_cli_callback fp) {
	t_rpc_req_ret ret;
	
	if(g_rpcClientInfo[item].stat == RPC_CLIENT_READY) {
		const t_rpc_server_stat srvStat = rpcServerStatRead(item);
		if(srvStat == RPC_SERVER_READY) {
			rpcClientStatWrite(item, RPC_CLIENT_REQUEST);
			g_rpcClientInfo[item].stat = RPC_CLIENT_REQUEST;
			g_rpcClientInfo[item].fp = fp;
			ret = RPC_REQUEST_OK;
		} else {
			ret = RPC_REQUEST_ERR;
		}
	} else {
		ret = RPC_REQUEST_ERR;
	}
	return ret;
}
