#ifndef __REVO_GPS_UART_H__
#define __REVO_GPS_UART_H__

#include "revo_gps_config.h"

typedef struct {
	revo_u32 r_pos, w_pos;
	revo_u32 left_space;
	revo_mutex gps_fifo_mutex;
	revo_char buffer[REVO_GPS_UART_FIFO_SIZE];
} revo_gps_uart_fifo_t;

revo_u32 revo_gps_uart_init(void);
revo_u32 revo_gps_uart_write(char *buf, revo_u32 len);
revo_s32 revo_gps_uart_read_char(revo_char *data_buf);

#endif

