
#ifndef __RPC_SERVER_H__
#define __RPC_SERVER_H__

#include "type.h"
#include "rpc_item.h"

typedef enum {
	RPC_SERVER_READY = 0,
	RPC_SERVER_ACCEPT,
	RPC_SERVER_UNDEFINED,
	RPC_SERVER_ERROR,
	RPC_SERVER_FINISH
}t_rpc_server_stat;

typedef enum {
	RPC_PROCESS_REDAY = 0,
	RPC_PROCESS_PENDING,
	RPC_PROCESS_OK,
	RPC_PROCESS_ERROR	
}t_rpc_process_stat;

typedef struct {
	t_rpc_server_stat stat;
	t_rpc_process_stat processStat;
	t_fp_callback fp;
}t_rpc_server_info;

void rpcRunServerObserver(void);
void rpcRegisterService(t_rpc_item item, t_fp_callback fp);
void rpcSetProcessStat(t_rpc_item item, t_rpc_process_stat stat);

#endif
