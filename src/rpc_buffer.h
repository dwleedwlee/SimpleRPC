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

#ifndef __RPC_BUFFER_H__
#define __RPC_BUFFER_H__

#include "type.h"
#include "rpc_item.h"
#include "rpc_server.h"
#include "rpc_client.h"

typedef struct {
	t_rpc_server_stat srvStat;
	t_rpc_client_stat cliStat;
}t_rpc_stat_info;

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
