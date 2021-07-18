
#ifndef __RPC_BUFFER_H__
#define __RPC_BUFFER_H__

#include "type.h"
#include "rpc_item.h"
#include "rpc_server.h"
#include "rpc_client.h"

#define RPC_DATA_BUF_SIZE	(32)

typedef struct {
	t_rpc_server_stat srvStat;
	t_rpc_client_stat cliStat;
}t_rpc_stat_info;

typedef struct {
	uint8 buf[RPC_DATA_BUF_SIZE];
	uint8 size;
}t_rpc_buf;

typedef struct {
	t_rpc_buf reqDataBuf;
	t_rpc_buf rspDataBuf;
}t_rpc_buf_info;

t_rpc_server_stat rpcServerStatRead(t_rpc_item item);
void rpcServerStatWrite(t_rpc_item item, t_rpc_server_stat stat);
t_rpc_client_stat rpcClientStatRead(t_rpc_item item);
void rpcClientStatWrite(t_rpc_item item, t_rpc_client_stat stat);
void rpcReqDataRead(t_rpc_item item, uint8 *buf);
void rpcReqDataWrite(t_rpc_item item, uint8 *buf, uint8 size);
void rpcRspDataRead(t_rpc_item item, uint8 *buf);
void rpcRspDataWrite(t_rpc_item item, uint8 *buf, uint8 size);

#endif
