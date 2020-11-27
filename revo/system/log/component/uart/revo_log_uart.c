#include "revo_os.h"
#include "revo_uart.h"
#include "revo_log_config.h"
#include "revo_log_uart.h"

static void revo_log_uart_callback(uint32 event)
{
	revo_u32 cnt;
	revo_char buf[129] = {0};

	revo_printf("%s() event = %d", __func__, event);
	switch (event)
	{
		case REVO_UART_DATA_TO_READ: {      
			cnt = revo_uart_read(REVO_LOG_UART_COM, buf, 128);            
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

static void revo_log_uart_cfg_params(revo_uart_params_t *params)
{
	params->baud_rate = REVO_LOG_UART_BAUD;
	params->byte_size = UART_EIGHT_BITS;
	params->flow_ctl = UART_NO_FLOW_CTL;
	params->parity = REVO_FALSE;
	params->parity_enable = UART_PARITY_DISABLE;
	params->stop_bit = UART_ONE_STOP_BIT;
}

revo_u32 revo_log_uart_write(revo_char *buf, revo_u32 len)
{
	revo_uart_write(REVO_LOG_UART_COM, buf, len);
}

revo_u32 revo_log_uart_open(void)
{
	revo_uart_params_t params;
	revo_u32 ret;

	revo_log_uart_cfg_params(&params);
	ret = revo_uart_open(REVO_LOG_UART_COM, &params, revo_log_uart_callback);
	if(ret == REVO_FAIL)
	{
		revo_printf("%s() Error: fail to open COM%d", REVO_LOG_UART_COM);
	}

	return ret;
}

void revo_log_uart_close(void)
{
	revo_uart_close(REVO_LOG_UART_COM);
}

