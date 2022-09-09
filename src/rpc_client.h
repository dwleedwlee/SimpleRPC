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
	/* linked array */
	t_rpc_item prev;
	t_rpc_item next;
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
