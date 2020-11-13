#include "test_type.h"

#ifdef __cplusplus
	extern "C"
{
#endif

/**
* --------------------------------------------------------------------------------------------------------------
* THREAD TEST
* --------------------------------------------------------------------------------------------------------------
*/

uint32 thread_create(revo_task_params_t *params) {
	
return SCI_CreateAppThread(
				params->task_name, 
				params->queue_name, 
				params->entry, 
				params->argc, 
				params->argv, 
				params->stack_size, 
				params->queue_num, 
				params->priority, 
				params->preempt, 
				params->auto_start);
}

void example_context(uint32 argc, void *argv) {
	while(1) {
		printf("%d\n", 1);
	}
}

void task_test_create(void) {
	revo_task_params_t params;
	
	params.task_name = "thread_test_task";
	params.queue_name = "thread_test_queue";
	params.entry = example_context;
	params.argc = 0;
	params.argv = 0;
	params.queue_num = TEST_QUEUE_NUM;
	params.stack_size = TEST_STACK_SIZE;
	params.preempt = TASK_PREEMPT;
	params.priority = REVO_TASK_PRIORITY_0;
	params.auto_start = TASK_AUTO_START;

	thread_create(&params);
}
	

/**
* --------------------------------------------------------------------------------------------------------------
* UART TEST
* --------------------------------------------------------------------------------------------------------------
*/


void uart_example(uint32 argc, void *argv) {
	uint result;
	UART_INIT_PARA_T params;
	uart_params_cfg(&params);
	result = uart_open_func(UART_PORT_0, &params, uart_call_back(0));

	if(result == UART_OPERATION_FAIL) {
		revo_printf("%s function error can't open UART_PORT_0  Line : %d", __func__, __LINE__);
		return;
	}

	while(1) {
		revo_printf("%s: UART_PORT_0 is open  result = %d", __func__, result);

		UART_WriteData(UART_PORT_0, "123456789", 9);
		SCI_SLEEP(2000);
	}
}


void uart_task_create(void) {
	revo_task_params_t params;
	
	params.task_name = "uart_test_task";
	params.queue_name = "uart_test_queue";
	params.entry = uart_example;
	params.argc = 0;
	params.argv = 0;
	params.queue_num = TEST_QUEUE_NUM;
	params.stack_size = TEST_STACK_SIZE;
	params.preempt = TASK_PREEMPT;
	params.priority = REVO_TASK_PRIORITY_0;
	params.auto_start = TASK_AUTO_START;

	thread_create(&params);
}

void uart_params_cfg(UART_INIT_PARA_T *params) {
	params->tx_watermark = 96;
	params->rx_watermark = 64;
	params->baud_rate = BAUD_115200;
	params->parity = EVEN_PARITY;
	params->parity_enable = PARITY_DISABLE;
	params->byte_size = EIGHT_BITS;
	params->stop_bits = ONE_STOP_BIT;
	params->flow_control = NO_FLOW_CONTROL;
	params->ds_wakeup_en = DS_WAKEUP_DISABLE;
}

LOCAL uint32 uart_port0_callback(uint32 event) {
	return 0;
}

UART_OPERATION_INFO uart_open_func(UART_PORT_NUM port, UART_INIT_PARA_T *params, UartCallback callback) {

	return UART_OPERATION_SUCCESS;
}

LOCAL UartCallback uart_callback_map(UART_PORT_NUM port) {

} 

/**
* --------------------------------------------------------------------------------------------------------------
* PRINT FUNC
* --------------------------------------------------------------------------------------------------------------
*/
void revo_printf(const char *fmt, ...) {
	uint8 str[REVO_LOG_BUF_SIZE];
	uint32 len;
	va_list args;
	
	memset(str, 0, REVO_LOG_BUF_SIZE);
	va_start(args, fmt);
	len = vsnprintf(str, REVO_LOG_BUF_SIZE, fmt, args);
	if(len > REVO_LOG_BUF_SIZE) {
		SCI_TRACE_LOW(REVO_LOG_TAG"ERROR:%s print buf outsize!", __func__);
	}
	va_end(args);
}


/**
* --------------------------------------------------------------------------------------------------------------
* EXAMPLE FUNC
* --------------------------------------------------------------------------------------------------------------
*/
void example_test_init(void) {

	//task_test_create();	
	uart_task_create();
}



#ifdef __cplusplus
}
#endif

























