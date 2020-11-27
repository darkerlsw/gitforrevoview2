#include "revo_typedef.h"
#include "revo_os.h"
#include "revo_uart.h"
#include "revo_gps_uart.h"
#include "revo_log.h"

revo_gps_uart_fifo_t uart_fifo;

static void revo_gps_uart_callback(uint32 event)
{
	revo_u32 cnt = 0;
	revo_gps_uart_fifo_t *fifo = &uart_fifo;
	revo_char datalog[257] = {0};

	switch (event)
	{
		case REVO_UART_DATA_TO_READ: {
			revo_mutex_lock(fifo->gps_fifo_mutex);
			
			while(fifo->left_space > 0)
			{
				if(revo_uart_read(REVO_GPS_UART_COM, &fifo->buffer[fifo->w_pos], 1) == 1)
				{
					datalog[cnt] = fifo->buffer[fifo->w_pos];
						
					cnt++;
					fifo->w_pos++;
					fifo->left_space--;
					if(fifo->w_pos >= REVO_GPS_UART_FIFO_SIZE)
						fifo->w_pos = 0;
				}else{
					break;
				}
			}
			
			revo_mutex_unlock(fifo->gps_fifo_mutex);
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

static void revo_gps_uart_fifo_init(revo_gps_uart_fifo_t *p)
{
	p->gps_fifo_mutex = revo_mutex_create();
	p->left_space = REVO_GPS_UART_FIFO_SIZE;
	p->r_pos = 0;
	p->w_pos = 0;
}

static void revo_gps_cfg_uart_com(revo_uart_params_t *params)
{
	params->baud_rate = REVO_GPS_UART_BAUD;
	params->byte_size = UART_EIGHT_BITS;
	params->flow_ctl = UART_NO_FLOW_CTL;
	params->parity = REVO_FALSE;
	params->parity_enable = UART_PARITY_DISABLE;
	params->stop_bit = UART_ONE_STOP_BIT;
}

revo_s32 revo_gps_uart_read_char(revo_char *data_buf)
{
	revo_gps_uart_fifo_t *fifo = &uart_fifo;

	revo_mutex_lock(fifo->gps_fifo_mutex);

	if(fifo->left_space < REVO_GPS_UART_FIFO_SIZE)
	{
		*data_buf = fifo->buffer[fifo->r_pos];
		fifo->buffer[fifo->r_pos] = '0';
		fifo->r_pos++;
		fifo->left_space++;
		if(fifo->r_pos >= REVO_GPS_UART_FIFO_SIZE)
			fifo->r_pos = 0;
	}else{
		revo_mutex_unlock(fifo->gps_fifo_mutex);
		return REVO_FAIL;
	}
	
	revo_mutex_unlock(fifo->gps_fifo_mutex);
	
	return REVO_SUCCESS;
}

revo_u32 revo_gps_uart_write(char *buf, revo_u32 len)
{
	return revo_uart_write(REVO_GPS_UART_COM, buf, len);
}

revo_u32 revo_gps_uart_init(void)
{
	revo_u32 ret;
	revo_uart_params_t uart_params;

	revo_gps_uart_fifo_init(&uart_fifo);
	revo_gps_cfg_uart_com(&uart_params);
	ret = revo_uart_open(REVO_GPS_UART_COM, &uart_params, revo_gps_uart_callback);
	if(ret == REVO_FAIL)
	{
		REVO_LOGE(TAG, "%s() Error: fail to open uart com!", __func__);
	}

	return ret;
}

