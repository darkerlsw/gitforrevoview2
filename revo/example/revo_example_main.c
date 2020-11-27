#include "revo_os.h"
#include "revo_log.h"

void revo_example_init(void)
{
	//revo_uart_task_create();
	revo_log_init();
	revo_gps_open();
}
