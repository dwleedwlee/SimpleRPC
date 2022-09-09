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

#ifndef __RPC_ITEM_H__
#define __RPC_ITEM_H__

#include "type.h"

typedef enum {
	RPC_GENERAL_ITEM_START = 0,
	RPC_GENERAL_ITEM_1,
	RPC_GENERAL_ITEM_2,
	RPC_GENERAL_ITEM_3,
	RPC_GENERAL_ITEM_4,
	RPC_GENERAL_ITEM_5,
	RPC_GENERAL_ITEM_6,
	RPC_GENERAL_ITEM_7,
	RPC_GENERAL_ITEM_8,
	RPC_GENERAL_ITEM_MAX
}t_rpc_item;

#define RPC_DATA_BUF_SIZE	(32)

typedef struct {
	uint8 buf[RPC_DATA_BUF_SIZE];
	uint8 size;
}t_rpc_buf;

#endif
