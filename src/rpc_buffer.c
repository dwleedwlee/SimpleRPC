#include <stdio.h>
#include <string.h>
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"


static t_rpc_server_stat gServerStatMailbox = RPC_SERVER_READY;
static t_rpc_client_stat gClientStatMailbox = RPC_CLIENT_READY;
static t_rpc_buf gReqDataBuf;
static t_rpc_buf gRspDataBuf;


t_rpc_server_stat rpcServerStatRead() {
	return gServerStatMailbox;
}

void rpcServerStatWrite(t_rpc_server_stat stat) {
	gServerStatMailbox = stat;
}

t_rpc_client_stat rpcClientStatRead() {
	return gClientStatMailbox;
}

void rpcClientStatWrite(t_rpc_client_stat stat) {
	gClientStatMailbox = stat;
}

void rpcReqDataRead(uint8 *buf) {
	memcpy(buf, gReqDataBuf.buf, gReqDataBuf.size);
}

void rpcReqDataWrite(uint8 *buf, uint8 size) {
	memcpy(gReqDataBuf.buf, buf, size);
	gReqDataBuf.size = size;
}

void rpcRspDataRead(uint8 *buf) {
	memcpy(buf, gRspDataBuf.buf, gRspDataBuf.size);
}

void rpcRspDataWrite(uint8 *buf, uint8 size) {
	memcpy(gRspDataBuf.buf, buf, size);
	gRspDataBuf.size = size;
}
