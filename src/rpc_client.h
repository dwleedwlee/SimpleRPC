
#ifndef __RPC_CLIENT_H__
#define __RPC_CLIENT_H__

#include "type.h"

typedef enum {
	RPC_CLIENT_READY = 0,
	RPC_CLIENT_REQUEST
}t_rpc_client_stat;

typedef struct {
	t_rpc_client_stat stat;
	t_fp_callback fp;
}t_rpc_client_info;

typedef enum {
	RPC_REQUEST_OK = 0,
	RPC_REQUEST_ERR
}t_rpc_req_ret;

void rpcRunClientObserver(void);
t_rpc_req_ret rpcRequestService(t_fp_callback fp);

#endif
