
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
	fflush(stdout);
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
	fflush(stdout);
}

void processRequest(t_rpc_item item, uint8 *req_data) {
	printf("\nServer[%d]: %s", item, req_data);
	fflush(stdout);
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
	t_rpc_buf reqData[5] = {
		{"RPC REQUEST - START", sizeof("RPC REQUEST - START")+1},
		{"RPC REQUEST - Did you get?", sizeof("RPC REQUEST - Did you get?")+1},
		{"RPC REQUEST - FINISH", sizeof("RPC REQUEST - FINISH")+1},
		{"CREATEIVE #### $$$$", sizeof("CREATEIVE #### $$$$")+1},
		{"0103452345_GOT_IT?_____OK", sizeof("0103452345_GOT_IT?_____OK")+1}
	};
	
	rpcInitServerInfo();
	rpcInitClientInfo();
	
	if(pthread_create(&thread_t, NULL, runObserverThread, NULL) < 0) {
		perror("Couldn't run observer!!!");
		exit(0);
	}
	
	rpcRegisterService(RPC_GENERAL_ITEM_3, processRequest);
	rpcRegisterService(RPC_GENERAL_ITEM_5, processRequest);
	rpcRegisterService(RPC_GENERAL_ITEM_7, processRequest);
	
	
	if(rpcRequestService(RPC_GENERAL_ITEM_3, printResponse, &reqData[0]) == RPC_REQUEST_OK) {
		
	}
	usleep(10000);
	
	
	if(rpcRequestService(RPC_GENERAL_ITEM_5, printResponse, &reqData[1]) == RPC_REQUEST_OK) {
		
	}		
	usleep(10000);
	
	
	if(rpcRequestService(RPC_GENERAL_ITEM_7, reversePrintResponse, &reqData[2]) == RPC_REQUEST_OK) {
		
	}		
	usleep(10000);
	
	if(rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &reqData[0]) == RPC_REQUEST_OK) {
		rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &reqData[3]);		
	}
	usleep(10000);
	
	if(rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[1]) == RPC_REQUEST_OK) {
		rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[3]);
		rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[2]);
		rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[1]);
		rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[0]);
		rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[4]);
	}
	
	pthread_join(thread_t, (void **)&status);

	return 0;
}
