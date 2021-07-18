
#include <stdio.h>
#include "rpc_item.h"
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"


void printResponse(t_rpc_item item) {
	uint8 buf[RPC_DATA_BUF_SIZE];
	rpcRspDataRead(item, buf);
	printf("[%d]: %s", item, buf);
}

void writeData(t_rpc_item item) {
	uint8 buf[] = "RPC IS RUNNING!!!\n";
	rpcRspDataWrite(item, buf, sizeof(buf));
	rpcSetProcessStat(item, RPC_PROCESS_OK);
}

int main (void) {
	
	rpcRegisterService(RPC_GENERAL_ITEM_3, writeData);
	rpcRequestService(RPC_GENERAL_ITEM_3, printResponse);
	
	rpcRegisterService(RPC_GENERAL_ITEM_5, writeData);
	rpcRequestService(RPC_GENERAL_ITEM_5, printResponse);
	
	while(1) {
		rpcRunServerObserver();
		rpcRunClientObserver();
	}
	
	return 0;
}
