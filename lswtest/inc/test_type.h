#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "priority_system.h"
#include "com_drvapi.h"
#include "os_api.h"
#include "sci_api.h"
#include "stdarg.h"
#include "gpio_prod_cfg.h"
#include "revo_os.h"



//#include "pinmap_cfg.c"    //GPIO引脚注册
//#include "gpio_cfg.c"      //GPIO功能注册
//#include "gpio_prod.c"     //GPIO功能函数接口实现gpio

#ifdef __cplusplus
	extern "C"
}
#endif

#define REVO_LOG_BUF_SIZE          512
#define REVO_LOG_TAG "[REVO]"
//#define REVO_TASK_PRIORITY_0       27
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

typedef enum {
	IS_OFF,
	IS_ON
} LED_CURRENT_STATUS;

/*
typedef enum {
	LED_OFF,
	LED_ON,
	LED_FLICKER
} LED_MODEL_OPTIONS;
*/	
typedef enum {
	RED_LED = 10,
	GREEN_LED,
	BLUE_LED
} LED_COLOR;
/*
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
*/

typedef struct {
	UART_PORT_NUM port;
	UartCallback callback_handle_func;
	UART_EVENT_TYPE event;
} CALL_BACK_PARAMS;

typedef struct {
	revo_char *thread_name;
	uint32 thread_id;
} thread_name_to_id_t;


//链表节点
typedef struct revo_list_elements {
    struct revo_list_elements *prev;
    struct revo_list_elements *next;
    void *content;
} revo_node_t;

//链表
typedef struct {
    revo_node_t *first;
    revo_node_t *last;
    revo_node_t *current;
    revo_s32 count;
    revo_u32 size;
} revo_linked_list_t;

void example_test_init(void);
revo_u32 revo_get_thread_id(revo_char *name);
void uart_params_cfg(UART_INIT_PARA_T *params);
uint32 uart_call_back(uint32 event);
UART_OPERATION_INFO uart_open_func(UART_PORT_NUM port, UART_INIT_PARA_T *params, UartCallback callback);
void revoex_printf(const char *fmt, ...);
void revo_linked_list_append(revo_linked_list_t *list, void *content, revo_u32 size);


#ifdef __cplusplus
}
#endif
#endif
