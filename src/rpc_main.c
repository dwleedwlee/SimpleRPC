
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <windows.h>
#include "rpc_item.h"
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"
#include "rpc_util.h"

#define NUMBER_OBSERVER_THREAD	(2)
#define NUMBER_CLIENT_THREAD	(6)
#define NUMBER_REQUEST_DATA		(5)

static t_rpc_buf g_reqData[NUMBER_REQUEST_DATA] = {
	{"RPC REQUEST - START", sizeof("RPC REQUEST - START")+1},
	{"RPC REQUEST - Did you get?", sizeof("RPC REQUEST - Did you get?")+1},
	{"RPC REQUEST - FINISH", sizeof("RPC REQUEST - FINISH")+1},
	{"CREATEIVE #### $$$$", sizeof("CREATEIVE #### $$$$")+1},
	{"0103452345_GOT_IT?_____OK", sizeof("0103452345_GOT_IT?_____OK")+1}
};

static void printResponse(t_rpc_item item) {
	uint8 buf[RPC_DATA_BUF_SIZE];
	rpcRspDataRead(item, buf);
	printf("\nClient[%d]: %s", item, buf);
	fflush(stdout);
}

static void reversePrintResponse(t_rpc_item item) {
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

static void processRequest(t_rpc_item item, uint8 *reqData) {
	printf("\nServer[%d]: %s", item, reqData);
	uint8 i;
	for(i = 0; i < NUMBER_REQUEST_DATA; i++) {
		if(strncmp(g_reqData[i].buf, reqData, g_reqData[i].size) == 0) {
			break;
		}
	}
	if(i >= NUMBER_REQUEST_DATA) {
		printf("\nError : Request data is not correct !!!\n");
		exit(0);
	}
	
	fflush(stdout);
	uint8 buf[] = "RPC IS RUNNING!!!";
	rpcRspDataWrite(item, buf, sizeof(buf));
	rpcSetProcessStat(item, RPC_PROCESS_OK);
}

static void *runClientThread1(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_1, printResponse, &g_reqData[0]) == RPC_REQUEST_OK) {
			
		}
		MS_SLEEP(11);
	}
	return NULL;
}

static void *runClientThread2(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_2, printResponse, &g_reqData[1]) == RPC_REQUEST_OK) {
			
		}		
		MS_SLEEP(17);
	}
	return NULL;
}

static void *runClientThread3(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &g_reqData[2]) == RPC_REQUEST_OK) {
		
		}		
		MS_SLEEP(7);
	}
	return NULL;
}

static void *runClientThread4(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_4, reversePrintResponse, &g_reqData[0]) == RPC_REQUEST_OK) {
			if(rpcRequestService(RPC_GENERAL_ITEM_4, reversePrintResponse, &g_reqData[3]) == RPC_REQUEST_OK) {
				
			}	
		}
		MS_SLEEP(5);
	}
	return NULL;
}

static void *runClientThread5(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &g_reqData[1]) == RPC_REQUEST_OK) {
			rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &g_reqData[3]);
			rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &g_reqData[2]);
			rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &g_reqData[1]);
			rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &g_reqData[0]);
			rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &g_reqData[4]);
		}
		MS_SLEEP(3);
	}
	return NULL;
}

static void *runClientThread6(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_6, reversePrintResponse, &g_reqData[3]) == RPC_REQUEST_OK) {
			MS_SLEEP(2);
		}
				
		if(rpcRequestService(RPC_GENERAL_ITEM_6, reversePrintResponse, &g_reqData[4]) == RPC_REQUEST_OK) {
			MS_SLEEP(2);
		}
	}
	return NULL;
}


static void *runServerObserverThread(void *arg) {

	while(1) {		
		rpcRunServerObserver();
	}
	
	return NULL;
}

static void *runClientObserverThread(void *arg) {

	while(1) {		
		rpcRunClientObserver();
	}
	
	return NULL;
}

int main (void) {
	pthread_t observer_thread[NUMBER_OBSERVER_THREAD];
	pthread_t client_thread[NUMBER_CLIENT_THREAD];
	
	void *(*fpObserverThread[NUMBER_OBSERVER_THREAD])(void *arg) = {
		runServerObserverThread,
		runClientObserverThread
	};	
	uint8 observerThreadStatus[NUMBER_OBSERVER_THREAD];	
	
	void *(*fpClientThread[NUMBER_CLIENT_THREAD])(void *arg) = {
		runClientThread1,
		runClientThread2,
		runClientThread3,
		runClientThread4,
		runClientThread5,
		runClientThread6
	};
	
	rpcInitServerInfo();
	rpcInitClientInfo();	
	
	uint8 i;
	for(i = 0; i < RPC_GENERAL_ITEM_MAX; i++) {
		rpcRegisterService(i, processRequest);
	}

	for(i = 0; i < NUMBER_OBSERVER_THREAD; i++) {
		if(pthread_create(&observer_thread[i], NULL, fpObserverThread[i], NULL) < 0) {
			printf("Couldn't run observer %d!!!\n", i);
			exit(0);
		}
	}
	
	for(i = 0; i < NUMBER_CLIENT_THREAD; i++) {
		if(pthread_create(&client_thread[i], NULL, fpClientThread[i], NULL) < 0) {
			printf("Couldn't run client %d!!!\n", i);
			exit(0);
		}
	}	
	
	for(i = 0; i < NUMBER_OBSERVER_THREAD; i++) {
		pthread_join(observer_thread[i], (void **)&observerThreadStatus[i]);
	}

	return 0;
}
