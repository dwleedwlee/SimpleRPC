
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
	printf("\n[%d]: %s", item, buf);
}

void reservePrintResponse(t_rpc_item item) {
	uint8 buf[RPC_DATA_BUF_SIZE];
	uint8 idx = 0;
	sint8 i = 0;
	rpcRspDataRead(item, buf);
	while(buf[idx] != '\0' && idx < RPC_DATA_BUF_SIZE) {
		idx++;
	}
	
	printf("\n[%d]: ", item);
	for(i = (idx - 1); i >= 0; i--) {
		printf("%c", buf[i]);
	}
}

void writeData(t_rpc_item item) {
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
	
	if(pthread_create(&thread_t, NULL, runObserverThread, NULL) < 0) {
		perror("Couldn't run observer!!!");
		exit(0);
	}
	
	rpcRegisterService(RPC_GENERAL_ITEM_3, writeData);
	rpcRegisterService(RPC_GENERAL_ITEM_5, writeData);
	rpcRegisterService(RPC_GENERAL_ITEM_7, writeData);
	
	rpcRequestService(RPC_GENERAL_ITEM_3, printResponse);	
	usleep(10000);	
	rpcRequestService(RPC_GENERAL_ITEM_5, printResponse);	
	usleep(10000);	
	rpcRequestService(RPC_GENERAL_ITEM_7, reservePrintResponse);	
	usleep(10000);	
	rpcRequestService(RPC_GENERAL_ITEM_5, reservePrintResponse);
	usleep(10000);
	rpcRequestService(RPC_GENERAL_ITEM_3, reservePrintResponse);
	
	pthread_join(thread_t, (void **)&status);

	return 0;
}
