#include "test_type.h"
#include "revo_log.h"
#include "revo_led.h"
#include "revo_gpio.h"
#include "linked_list.h"
#include "revo_keypad.h"

#ifdef __cplusplus
	extern "C"
{
#endif

/**
* --------------------------------------------------------------------------------------------------------------
* THREAD TEST
* --------------------------------------------------------------------------------------------------------------
*/

#define TAG "led"
uint32 key_thread_id = 0;
revo_linked_list_t thread_list = {0};
thread_name_to_id_t *nti = NULL;
revo_node_t *loop = NULL;
uint32 key_p_id = 0; 
revo_key_value_e key_val = {0};

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

static void example_context(uint32 argc, void *argv) {
	while(1) {
		revoex_printf("%d\n", 1);
	}
}

static void task_test_create(void) {
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


static void uart_example(uint32 argc, void *argv) {
	uint result;
	UART_INIT_PARA_T params;
	uart_params_cfg(&params);
	result = uart_open_func(UART_PORT_0, &params, uart_call_back(0));

	if(result == UART_OPERATION_FAIL) {
		revoex_printf("%s function error can't open UART_PORT_0  Line : %d", __func__, __LINE__);
		return;
	}

	while(1) {
		revoex_printf("%s: UART_PORT_0 is open  result = %d", __func__, result);

		UART_WriteData(UART_PORT_0, "0123456789", 10);
		SCI_SLEEP(2000);
	}
}


static void uart_task_create(void) {
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

uint32 uart_call_back(uint32 event) {

	return 0;
};


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
* LED TEST
* GPIO10 -> RED
* GPIO11 -> GREEN
* GPIO12 -> BLUE
* --------------------------------------------------------------------------------------------------------------
*/

static LED_CURRENT_STATUS led_status_set(LED_COLOR color, LED_CURRENT_STATUS status) {
	REVO_LOGI(TAG, "%s() the function is running, %d led current status is %d", __func__, color, status);
	revoex_printf(REVO_LOG_TAG"%d led current status is %d.", color, status);
	GPIO_SetValue(color, status);
	return status;
}


static void led_example(uint32 argc, void *argv) {
/*
 	int led_port = 10;
 	int i = 0;

	revo_log_init();
	while(1)
	{
		REVO_LOGI(TAG, "%s() the function is running", __func__);
		
		//revoex_printf("running!");
		if(!i)
		{
			GPIO_SetValue(led_port, 1);
			i = 1;
			REVO_LOGI(TAG, "%s() on, led_port = %d", __func__, led_port);
		}
		else
		{
			REVO_LOGI(TAG, "%s() off, led_port = %d", __func__, led_port);
			GPIO_SetValue(led_port++, 0);
			i = 0;
			if(led_port > 12)
				led_port = 10;
		}
		
		SCI_SLEEP(500);
	}
*/
	revo_led_init();
	revo_led_on(LED_G);
	
}


static void led_task_create(void) {
	revo_task_params_t params;
	params.task_name = "led_test_task";
	params.queue_name = "led_test_queue";
	params.entry = led_example;
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
* KEY TEST
* --------------------------------------------------------------------------------------------------------------
*/
static revo_s32 revo_strcmp(char *p1, char *p2)
{
	revo_s32 i = 0, n = 0;
	while (*p1 == *p2) { 
		p1++; 
		p2++; 
	}
	
	n = *p1 - *p2;
	
	if(n > 25 || n < -25)
		return 0;
	else 
		return n;
}

static void key_test(uint32 argc, void *argv) {
	uint32 signal, id;	
	
	while(1) {
		signal = (uint32)SCI_GetSignal(key_thread_id);
		REVO_LOGI("revo", "SCI_GetSignal = %d", signal);
		
		id = revo_get_thread_id("key_test_task"); //通过名字获取到的id
		REVO_LOGI("revo", "%s(): key_thread_id = %x ?= id = %x", __func__, key_thread_id, id);
		REVO_LOGI("revo", "%s(): id = %x, name = %s", __func__, nti->thread_id, nti->thread_name);

	}
}


static void key_press_callback() {
	REVO_LOGI("revo", "%s() key_val = %d", __func__, key_val);
	revo_task_msg_send("key_test_task", REVO_KEY_EVT_PRESS_HOLD, &key_val, sizeof(revo_key_value_e));
}


static void key_press_hold_thread_init() {
	key_p_id = revo_create_period_timer("key_press_hold", key_press_callback, NULL, 1000, SCI_NO_ACTIVATE);
}

static void key_press_hold_detection(revo_key_event_e event) {
	if(event == REVO_KEY_EVT_PRESS) {
		if(!revo_is_timer_active(key_p_id)) {
			revo_active_timer(key_p_id);
			REVO_LOGI("revo", "%s() thread is active", __func__);
		}
	
	} else if (event == REVO_KEY_EVT_PRESS_HOLD) {
		return;
	} else {
		if(revo_is_timer_active(key_p_id)) {
			revo_deactive_timer(key_p_id);
			//重新设置时间，清空之前定时器剩余的时间，防止时间溢出
			revo_change_timer(key_p_id, key_press_callback, 1000); 
			REVO_LOGI("revo", "%s() thread is deactive", __func__);
		}	
	}
}

static void key_example(revo_u32 argc, void *argv)
{
	revo_key_event_e event;
	revo_key_value_e value;
	revo_u32 ret;

	key_press_hold_thread_init();  //初始化timer
	
	while(1)
	{
		REVO_LOGI(TAG, "%s() running!", __func__);
		ret = revo_task_msg_recv("key_test_task", &event, &value, sizeof(revo_key_value_e));
		key_val = value;
		
		if(ret == REVO_FAIL)
		{
			REVO_LOGE(TAG, "%s() Error: fail to recv msg!", __func__);
			continue;
		}

		key_press_hold_detection(event);
		
		REVO_LOGI(TAG, "%s() event = %d, value = %d, key_val = %d", __func__, event, value, key_val);
		
		switch(event)
		{
			case REVO_KEY_EVT_PRESS:
			{				
				REVO_LOGE(TAG, "%s() REVO_KEY_EVT_PRESS", __func__);
				break;
			}

			case REVO_KEY_EVT_RELEASE:
			{

				REVO_LOGE(TAG, "%s() REVO_KEY_EVT_RELEASE", __func__);
				break;
			}

			case REVO_KEY_EVT_PRESS_HOLD:
			{
				REVO_LOGE(TAG, "%s() REVO_KEY_EVT_PRESS_HOLD", __func__);
				break;	
			}
			
			default:
				break;
		}
		
		//revo_task_sleep(500);
	}
}

static void key_task_create(void) {
	revo_task_params_t params;
	
	nti = malloc(sizeof(thread_name_to_id_t));
	
	params.task_name = "key_test_task";
	params.queue_name = "key_test_queue";
	params.entry = key_example;
	params.argc = 0;
	params.argv = 0;
	params.queue_num = TEST_QUEUE_NUM;
	params.stack_size = TEST_STACK_SIZE;
	params.preempt = TASK_PREEMPT;
	params.priority = REVO_TASK_PRIORITY_0;
	params.auto_start = TASK_AUTO_START;

	key_thread_id = thread_create(&params);
	
	nti->thread_id = key_thread_id;
	nti->thread_name = "key_test_task";
	
	revo_linked_list_append(&thread_list, (void *)nti, sizeof(thread_name_to_id_t));
}

revo_u32 revo_get_thread_id(revo_char *name) {	
	loop = thread_list.first;

	//REVO_LOGI("revo", "%d, name = %s, nti_addr = %x", 0000, nti->thread_name, nti);	
	//REVO_LOGI("revo", "FIRST = %x", thread_list.first->content);
	//REVO_LOGI("revo", "%d, name = %s", 1111, name);

	if(name == NULL || ((thread_name_to_id_t *)(loop->content))->thread_name == NULL) {
		//REVO_LOGI("revo", "%s() null pointer exception", __func__);
		return NULL;
	} 

	while(loop != NULL) {
		//REVO_LOGI("revo", "%d, name = %s", 2222, ((thread_name_to_id_t *)(loop->content))->thread_name);
		if(str_compare(name, ((thread_name_to_id_t *)(loop->content))->thread_name)) {
			//REVO_LOGI("revo", "%d", 3333);
			return ((thread_name_to_id_t *)(loop->content))->thread_id;
		}
		loop = loop->next;
	}

	return NULL;
}

/**
* --------------------------------------------------------------------------------------------------------------
* PRINT FUNC
* --------------------------------------------------------------------------------------------------------------
*/
void revoex_printf(const char *fmt, ...) {
	uint8 str[REVO_LOG_BUF_SIZE];
	uint32 len;
	va_list args;
	
	memset(str, 0, REVO_LOG_BUF_SIZE);
	va_start(args, fmt);
	len = vsnprintf(str, REVO_LOG_BUF_SIZE, fmt, args);
	if(len > REVO_LOG_BUF_SIZE) {
		SCI_TRACE_LOW(REVO_LOG_TAG"ERROR:%s print buf outsize!", __func__);
	}
	SCI_TRACE_LOW("%s", str);
	va_end(args);
}

/**
* --------------------------------------------------------------------------------------------------------------
* linked list
* --------------------------------------------------------------------------------------------------------------
*/


/*
 * 将元素添加到链表末尾
 */
static void revo_list_add_element(revo_linked_list_t *list, revo_node_t *new, void *content, revo_u32 size) {
    new->content = content;
    new->prev = list->last;
    new->next = NULL;
    if(list->first == NULL) {
        list->first = new;
    } else {
        list->last->next = new;
    }

    list->last = new;
    ++(list->count);
    list->size += size;
}

/*
 * 给链表节点分配内存并添加元素
 */
void revo_linked_list_append(revo_linked_list_t *list, void *content, revo_u32 size) {
    revo_node_t *new = malloc(sizeof(revo_node_t));
    revo_list_add_element(list, new, content, size);
}

/**
* --------------------------------------------------------------------------------------------------------------
* EXAMPLE FUNC
* --------------------------------------------------------------------------------------------------------------
*/
void example_test_init(void) {
	revo_log_init();
	//key_press_hold_thread_init();
	//task_test_create();	
	//uart_task_create();
	led_task_create();
	key_task_create();
}


#ifdef __cplusplus
}
#endif
