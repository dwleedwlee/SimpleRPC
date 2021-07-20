
#ifndef __RPC_CLIENT_H__
#define __RPC_CLIENT_H__

#include "type.h"
#include "rpc_item.h"

#define FEATURE_DUAL_BUFFER

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

#ifdef FEATURE_DUAL_BUFFER
typedef enum {
	DUAL_BUF_STAT_EMPTY = 0,
	DUAL_BUF_STAT_SINGLE,
	DUAL_BUF_STAT_FULL
}t_dual_buf_stat;

typedef struct {
	uint8 active_buf_idx;	/* 0 or 1 */
	t_dual_buf_stat stat;
	t_fp_cli_callback fp[2];
	t_rpc_buf rpc_buf[2];
}t_dual_buf_info;
#endif

void rpcInitClientInfo(void);
void rpcRunClientObserver(void);
t_rpc_req_ret rpcRequestService(t_rpc_item item, t_fp_cli_callback fp, t_rpc_buf *rpc_buf);

#endif
