#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "priority_system.h"
#include "com_drvapi.h"
#include "os_api.h"
#include "stdarg.h"


#ifdef __cplusplus
	extern "C"
}
#endif

#define REVO_LOG_BUF_SIZE          512
#define REVO_LOG_TAG "[REVO]"
#define REVO_TASK_PRIORITY_0       27
#define TEST_QUEUE_NUM             50
#define TEST_STACK_SIZE            16*1024

#define LOCAL static

typedef unsigned char		BOOLEAN;
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned long int	uint32;

typedef unsigned int		uint;

typedef signed char			int8;
typedef signed short		int16;
typedef signed long int		int32;

typedef enum {
	TASK_DONT_START,
	TASK_AUTO_START
} TASK_START_OPTION;

typedef enum {
	TASK_NO_PREEMPT,
	TASK_PREEMPT
} TASK_PREEMPT_OPTION;

typedef enum {
	REVO_EVENT_DATA_TO_READ,
	REVO_EVENT_WRITE_COMPLETE,
	REVO_EVENT_INIT_COMPLETE,
	REVO_EVENT_SHUTDOWN_COMPLETE
} UART_EVENT_TYPE;
	
typedef enum {
	UART_PORT_0,
	UART_PORT_1
} UART_PORT_NUM;

typedef enum {
	UART_OPERATION_SUCCESS,
	UART_OPERATION_FAIL
} UART_OPERATION_INFO;

typedef struct {
	char task_name;
	char queue_name;
	void (*entry)(uint32, void*);
	uint32 argc;
	void *argv;
	uint32 stack_size;
	uint32 queue_num;
	uint32 priority;
	uint32 preempt;
	uint32 auto_start;
} revo_task_params_t;

typedef struct {
	UART_PORT_NUM port;
	UartCallback callback_handle_func;
	UART_EVENT_TYPE event;
} CALL_BACK_PARAMS;


uint32 thread_create(revo_task_params_t *params);
void task_test_create(void);
void example_test_init(void);
void uart_task_create(void);
void uart_params_cfg(UART_INIT_PARA_T *params);
UART_OPERATION_INFO uart_open_func(UART_PORT_NUM port, UART_INIT_PARA_T *params, UartCallback callback);
uint32 uart_call_back(uint32 event);
void revo_printf(const char *fmt, ...);





#ifdef __cplusplus
}
#endif
#endif








