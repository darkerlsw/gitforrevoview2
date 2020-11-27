#ifndef __REVO_UART_H__
#define __REVO_UART_H__

#include "revo_os.h"
#include "com_drvapi.h"

typedef enum {
	REVO_UART_COM1 = 0,
	REVO_UART_COM2 = 1,
	REVO_UART_COM_MAX,
} uart_com_idx_e;

typedef enum {
	UART_BAUD_1200 = 0,
	UART_BAUD_2400,
	UART_BAUD_4800,
	UART_BAUD_9600,
	UART_BAUD_19200,
	UART_BAUD_38400,
	UART_BAUD_57600,
	UART_BAUD_115200,
	UART_BAUD_230400,
	UART_BAUD_470800,
	UART_BAUD_921600,
	UART_BAUD_2000000,
	UART_BAUD_3250000,
	UART_BAUD_8000000
} uart_baud_e;

typedef enum {
	UART_EVEN_PARITY = 0,
	UART_ODD_PARITY
} uart_parity_e;

typedef enum {
	UART_PARITY_DISABLE = 0,
	UART_PARITY_ENABLE
} uart_parity_set_e;

typedef enum {
	UART_FIVE_BITS = 0,
	UART_SIX_BITS,
	UART_SEVEN_BITS,
	UART_EIGHT_BITS
} uart_byte_size_e;

typedef enum {
	UART_STOP_BIT_UNUSED = 0,
	UART_ONE_STOP_BIT,
	UART_ONE_AND_HALF_BIT,
	UART_TWO_STOP_BIT
} uart_stop_bit_e;

typedef enum {
	UART_NO_FLOW_CTL = 0,
	UART_HW_FLOW_CTL,
	UART_SW_FLOW_CTL
} uart_flow_ctl_e;

typedef enum {
	REVO_UART_DATA_TO_READ = EVENT_DATA_TO_READ,
	REVO_UART_WRITE_CMPL = EVENT_WRITE_COMPLETE,
	REVO_UART_INIT_CMPL = EVENT_INIT_COMPLETE,
	REVO_UART_SHUTDOWN_CMPL = EVENT_SHUTDOWN_COMPLETE
} uart_event_e;

typedef struct {
	uart_baud_e baud_rate;
	uart_parity_e parity;
	uart_parity_set_e parity_enable;
	uart_byte_size_e byte_size;
	uart_stop_bit_e stop_bit;
	uart_flow_ctl_e flow_ctl;
} revo_uart_params_t;

typedef void (*revo_uart_callback_t)(uart_event_e event);

typedef struct {
	uart_com_idx_e com_idx;
	UartCallback system_callback;
	revo_uart_callback_t custom_callback;
} revo_uart_map_t;

revo_u32 revo_uart_open(uart_com_idx_e com_idx, revo_uart_params_t *params, revo_uart_callback_t callback);
revo_u32 revo_uart_close(uart_com_idx_e com_idx);
revo_u32 revo_uart_read(uart_com_idx_e com_idx, char *buf, revo_u32 len);
revo_u32 revo_uart_write(uart_com_idx_e com_idx, char *buf, revo_u32 len);

#endif

