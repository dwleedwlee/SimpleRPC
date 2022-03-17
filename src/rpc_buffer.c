/*
	The MIT License (MIT)
	
	Copyright (c) 2021 Dongwook Lee
	
	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
