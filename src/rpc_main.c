
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "rpc_item.h"
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"


void printResponse(t_rpc_item item) {
	uint8 buf[RPC_DATA_BUF_SIZE];
	rpcRspDataRead(item, buf);
	printf("\nClient[%d]: %s", item, buf);
}

void reversePrintResponse(t_rpc_item item) {
	uint8 buf[RPC_DATA_BUF_SIZE];
	uint8 idx = 0;
	sint8 i = 0;
	rpcRspDataRead(item, buf);
	while(buf[idx] != '\0' && idx < RPC_DATA_BUF_SIZE) {
		idx++;
	}
	
	printf("\nClient[%d]: ", item);
	for(i = (idx - 1); i >= 0; i--) {
		printf("%c", buf[i]);
	}
}

void processRequest(t_rpc_item item, uint8 *req_data) {
	printf("\nServer[%d]: %s", item, req_data);
	uint8 buf[] = "RPC IS RUNNING!!!";
	rpcRspDataWrite(item, buf, sizeof(buf));
	rpcSetProcessStat(item, RPC_PROCESS_OK);
}

void *runObserverThread(void *arg) {

	while(1) {
		rpcRunServerObserver();
		rpcRunClientObserver();
	}
	
	return NULL;
}

int main (void) {
	pthread_t thread_t;
	int status;
	uint8 reqData1[] = "RPC REQUEST BROWN";
	uint8 reqData2[] = "RPC REQUEST GREEN";
	uint8 reqData3[] = "RPC REQUEST BLUE";
	
	rpcInitServerInfo();
	rpcInitClientInfo();
	
	if(pthread_create(&thread_t, NULL, runObserverThread, NULL) < 0) {
		perror("Couldn't run observer!!!");
		exit(0);
	}
	
	rpcRegisterService(RPC_GENERAL_ITEM_3, processRequest);
	rpcRegisterService(RPC_GENERAL_ITEM_5, processRequest);
	rpcRegisterService(RPC_GENERAL_ITEM_7, processRequest);
	
	rpcReqDataWrite(RPC_GENERAL_ITEM_3, reqData1, sizeof(reqData1));
	rpcRequestService(RPC_GENERAL_ITEM_3, printResponse);	
	usleep(10000);
	rpcReqDataWrite(RPC_GENERAL_ITEM_5, reqData2, sizeof(reqData2));
	rpcRequestService(RPC_GENERAL_ITEM_5, printResponse);	
	usleep(10000);
	rpcReqDataWrite(RPC_GENERAL_ITEM_7, reqData3, sizeof(reqData3));
	rpcRequestService(RPC_GENERAL_ITEM_7, reversePrintResponse);	
	usleep(10000);	
	rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse);
	usleep(10000);
	rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse);
	
	pthread_join(thread_t, (void **)&status);

	return 0;
}
