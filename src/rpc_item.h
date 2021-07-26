
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
