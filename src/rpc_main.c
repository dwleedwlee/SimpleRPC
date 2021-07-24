
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <windows.h>
#include "rpc_item.h"
#include "rpc_server.h"
#include "rpc_client.h"
#include "rpc_buffer.h"

#define NUMBER_OBSERVER_THREAD	(2)
#define NUMBER_CLIENT_THREAD	(5)

#define MS_SLEEP(x)			(custom_usleep(x * 1000))

static t_rpc_buf reqData[5] = {
	{"RPC REQUEST - START", sizeof("RPC REQUEST - START")+1},
	{"RPC REQUEST - Did you get?", sizeof("RPC REQUEST - Did you get?")+1},
	{"RPC REQUEST - FINISH", sizeof("RPC REQUEST - FINISH")+1},
	{"CREATEIVE #### $$$$", sizeof("CREATEIVE #### $$$$")+1},
	{"0103452345_GOT_IT?_____OK", sizeof("0103452345_GOT_IT?_____OK")+1}
};

/* Refered to the sample usleep source code - BEGIN */
static void custom_usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}
/* Refered to the sample usleep source code - END */
	

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

static void processRequest(t_rpc_item item, uint8 *req_data) {
	printf("\nServer[%d]: %s", item, req_data);
	fflush(stdout);
	uint8 buf[] = "RPC IS RUNNING!!!";
	rpcRspDataWrite(item, buf, sizeof(buf));
	rpcSetProcessStat(item, RPC_PROCESS_OK);
}

static void *runClientThread1(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_3, printResponse, &reqData[0]) == RPC_REQUEST_OK) {
			
		}
		MS_SLEEP(3);
	}
	return NULL;
}

static void *runClientThread2(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_5, printResponse, &reqData[1]) == RPC_REQUEST_OK) {
			
		}		
		MS_SLEEP(5);
	}
	return NULL;
}

static void *runClientThread3(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_7, reversePrintResponse, &reqData[2]) == RPC_REQUEST_OK) {
		
		}		
		MS_SLEEP(7);
	}
	return NULL;
}

static void *runClientThread4(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &reqData[0]) == RPC_REQUEST_OK) {
			if(rpcRequestService(RPC_GENERAL_ITEM_5, reversePrintResponse, &reqData[3]) == RPC_REQUEST_OK) {
				
			}	
		}
		MS_SLEEP(10);
	}
	return NULL;
}

static void *runClientThread5(void *arg) {
	while(1) {
		if(rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[1]) == RPC_REQUEST_OK) {
			rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[3]);
			rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[2]);
			rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[1]);
			rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[0]);
			rpcRequestService(RPC_GENERAL_ITEM_3, reversePrintResponse, &reqData[4]);
		}
		MS_SLEEP(5);
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
		runClientThread5
	};
	
	rpcInitServerInfo();
	rpcInitClientInfo();	
	
	rpcRegisterService(RPC_GENERAL_ITEM_3, processRequest);
	rpcRegisterService(RPC_GENERAL_ITEM_5, processRequest);
	rpcRegisterService(RPC_GENERAL_ITEM_7, processRequest);

	uint8 i;
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
