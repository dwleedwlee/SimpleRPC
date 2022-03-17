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

#ifndef __RPC_SERVER_H__
#define __RPC_SERVER_H__

#include "type.h"
#include "rpc_item.h"

typedef enum {
	RPC_SERVER_READY = 0,
	RPC_SERVER_ACCEPT,
	RPC_SERVER_REJECT,
	RPC_SERVER_ERROR,
	RPC_SERVER_FINISH
}t_rpc_server_stat;

typedef enum {
	RPC_PROCESS_REDAY = 0,
	RPC_PROCESS_PENDING,
	RPC_PROCESS_OK,
	RPC_PROCESS_ERROR	
}t_rpc_process_stat;

typedef void (*t_fp_srv_callback)(t_rpc_item item, uint8 *buf);

typedef struct {
	t_rpc_server_stat stat;
	t_rpc_process_stat processStat;
	t_fp_srv_callback fp;
}t_rpc_server_info;

void rpcInitServerInfo(void);
void rpcRunServerObserver(void);
void rpcRegisterService(t_rpc_item item, t_fp_srv_callback fp);
void rpcSetProcessStat(t_rpc_item item, t_rpc_process_stat stat);

#endif
