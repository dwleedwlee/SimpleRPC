
#ifndef __RPC_CLIENT_H__
#define __RPC_CLIENT_H__

#include "type.h"
#include "rpc_item.h"

typedef enum {
	RPC_CLIENT_READY = 0,
	RPC_CLIENT_REQUEST
}t_rpc_client_stat;

typedef enum {
	RPC_REQUEST_OK = 0,
	RPC_REQUEST_ERR
}t_rpc_req_ret;

typedef void (*t_fp_cli_callback)(t_rpc_item item);

typedef struct {
	t_rpc_client_stat stat;
	t_fp_cli_callback fp;
}t_rpc_client_info;


void rpcInitClientInfo(void);
void rpcRunClientObserver(void);
t_rpc_req_ret rpcRequestService(t_rpc_item item, t_fp_cli_callback fp);

#endif
