#ifndef __REVO_LOG_UART_H__
#define __REVO_LOG_UART_H__

revo_u32 revo_log_uart_write(revo_char *buf, revo_u32 len);
revo_u32 revo_log_uart_open(void);
void revo_log_uart_close(void);

#endif
