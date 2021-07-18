#include <stdio.h>
#include <string.h>

#include "rpc_buffer.h"

static t_rpc_stat_info gRpcStatInfo[RPC_GENERAL_ITEM_MAX];
static t_rpc_buf_info gRpcBufInfo[RPC_GENERAL_ITEM_MAX];


t_rpc_server_stat rpcServerStatRead(t_rpc_item item) {
	return gRpcStatInfo[item].srvStat;
}

void rpcServerStatWrite(t_rpc_item item, t_rpc_server_stat stat) {
	gRpcStatInfo[item].srvStat = stat;
}

t_rpc_client_stat rpcClientStatRead(t_rpc_item item) {
	return gRpcStatInfo[item].cliStat;
}

void rpcClientStatWrite(t_rpc_item item, t_rpc_client_stat stat) {
	gRpcStatInfo[item].cliStat = stat;
}

void rpcReqDataRead(t_rpc_item item, uint8 *buf) {
	memcpy(buf, gRpcBufInfo[item].reqDataBuf.buf, gRpcBufInfo[item].reqDataBuf.size);
}

void rpcReqDataWrite(t_rpc_item item, uint8 *buf, uint8 size) {
	memcpy(gRpcBufInfo[item].reqDataBuf.buf, buf, size);
	gRpcBufInfo[item].reqDataBuf.size = size;	
}

void rpcRspDataRead(t_rpc_item item, uint8 *buf) {
	memcpy(buf, gRpcBufInfo[item].rspDataBuf.buf, gRpcBufInfo[item].rspDataBuf.size);
}

void rpcRspDataWrite(t_rpc_item item, uint8 *buf, uint8 size) {
	memcpy(gRpcBufInfo[item].rspDataBuf.buf, buf, size);
	gRpcBufInfo[item].rspDataBuf.size = size;
}
