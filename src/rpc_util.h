
#ifndef __RPC_UTIL_H__
#define __RPC_UTIL_H__

#include <unistd.h>

#define MS_SLEEP(x)			(custom_usleep(x * 1000))

void custom_usleep(__int64 usec);

#endif
