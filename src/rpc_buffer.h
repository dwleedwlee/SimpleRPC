
#ifndef __RPC_BUFFER_H__
#define __RPC_BUFFER_H__

#include "type.h"

typedef struct {
	uint8 buf[32];
	uint8 size;
}t_rpc_buf;

t_rpc_server_stat rpcServerStatRead(void);
void rpcServerStatWrite(t_rpc_server_stat stat);
t_rpc_client_stat rpcClientStatRead(void);
void rpcClientStatWrite(t_rpc_client_stat stat);
void rpcReqDataRead(uint8 *buf);
void rpcReqDataWrite(uint8 *buf, uint8 size);
void rpcRspDataRead(uint8 *buf);
void rpcRspDataWrite(uint8 *buf, uint8 size);

#endif
