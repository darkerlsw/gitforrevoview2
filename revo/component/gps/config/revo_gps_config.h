#ifndef __REVO_GPS_CONFIG_H__
#define __REVO_GPS_CONFIG_H__

#include "revo_uart.h"
#include "revo_os.h"

/* log */
#define TAG "gps"

/* uart */
#define REVO_GPS_UART_COM            REVO_UART_COM1
#define REVO_GPS_UART_BAUD           UART_BAUD_9600
#define REVO_GPS_UART_FIFO_SIZE      1024*8

/* task */
#define REVO_GPS_TASK_NAME           "REVO_GPS_TASK"
#define REVO_GPS_TASK_QNAME          "REVO_GPS_QUEUE"
#define REVO_GPS_TASK_STACK_SIZE     16*1024
#define REVO_GPS_TASK_QUEUE_NUM      50
#define REVO_GPS_TASK_PRIORITY       REVO_TASK_PRIORITY_2

#endif

