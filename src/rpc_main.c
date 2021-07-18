
#include <stdio.h>
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"


void printResponse(void) {
	uint8 buf[32];
	rpcRspDataRead(buf);
	printf("%s", buf);
}

void writeData(void) {
	uint8 buf[] = "RPC IS RUNNING!!!\n";
	rpcRspDataWrite(buf, sizeof(buf));
	rpcSetProcessStat(RPC_PROCESS_OK);
}

int main (void) {
	
	rpcRegisterService(writeData);
	rpcRequestService(printResponse);
	
	while(1) {
		rpcRunServerObserver();
		rpcRunClientObserver();
	}
	
	return 0;
}
