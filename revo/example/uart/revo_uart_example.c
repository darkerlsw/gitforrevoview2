#include "revo_os.h"
#include "revo_uart.h"

/* Task */
#define REVO_UART_EXAMPLE_TASK_STACK_SIZE            16*1024
#define REVO_UART_EXAMPLE_TASK_QUEUE_NUM             50

revo_u32 g_revo_uart_example_task_id;

static void revo_uart1_callback(uint32 event)
{
	revo_u32 cnt;
	revo_char buf[129] = {0};

	revo_printf("%s() event = %d", __func__, event);
	switch (event)
	{
		case REVO_UART_DATA_TO_READ: {      
			cnt = revo_uart_read(REVO_UART_COM1, buf, 128);            
			revo_printf("%s() get_data_cnt = %d,  read_data = %s", __func__, cnt, buf);
			break;
		}

		case REVO_UART_WRITE_CMPL:
			break;

		case REVO_UART_INIT_CMPL:
			break;

		case REVO_UART_SHUTDOWN_CMPL:
			break;

		default:
			break;
	}
}

void revo_cfg_uart_params(revo_uart_params_t *params)
{
	params->baud_rate = UART_BAUD_115200;
	params->byte_size = UART_EIGHT_BITS;
	params->flow_ctl = UART_NO_FLOW_CTL;
	params->parity = REVO_FALSE;
	params->parity_enable = UART_PARITY_DISABLE;
	params->stop_bit = UART_ONE_STOP_BIT;
}

void revo_uart_example_task(uint32 argc, void *argv)
{
	int cnt = 1;
	revo_u32 ret;
	revo_uart_params_t uart_params;

	revo_cfg_uart_params(&uart_params);
	ret = revo_uart_open(REVO_UART_COM1, &uart_params, revo_uart1_callback);
	if(ret == REVO_FAIL)
	{
		revo_printf("%s() Error: fail to open UART_COM1!", __func__);
		return;
	}
	
	while(1)
	{
		revo_printf("%s() running! init ret = %d", __func__, ret);
		if(cnt % 2 == 0)
		{
			revo_uart_write(REVO_UART_COM1, "123456789", 9);
		}
		cnt++;
		revo_task_sleep(2000);
	}
}

void revo_uart_task_create(void)
{
	revo_task_params_t task_params;

	task_params.task_name = "REVO_UART_EXAMPLE_TASK";
	task_params.queue_num = "REVO_UART_EXAMPLE_QUEUE";
	task_params.entry = revo_uart_example_task;
	task_params.argc = 0;
	task_params.argv = 0;
	task_params.stack_size = REVO_UART_EXAMPLE_TASK_STACK_SIZE;
	task_params.queue_num = REVO_UART_EXAMPLE_TASK_QUEUE_NUM;
	task_params.priority = REVO_TASK_PRIORITY_0;
	task_params.preempt = REVO_TASK_PREEMPT;
	task_params.auto_start = REVO_TASK_AUTO_START;
	g_revo_uart_example_task_id = revo_task_create(&task_params);
}

