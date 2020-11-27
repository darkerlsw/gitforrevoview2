#include "revo_os.h"
#include "revo_log.h"
#include "revo_keypad.h"

/* wrapper */
const revo_key_evt_map_t revo_key_evt_maptbl[] = {
	{REVO_KEY_EVT_PRESS, 0/*HAL_KEY_DOWN*/},
	{REVO_KEY_EVT_PRESS_HOLD, 1/*HAL_KEY_PRESSED*/},
	{REVO_KEY_EVT_RELEASE, 2/*HAL_KEY_UP*/},
};

const revo_key_val_map_t revo_key_val_maptbl[] = {
	{REVO_KEY_VOLUP, 49},
	{REVO_KEY_VOLDN, 52},
	{REVO_KEY_FUNC, 53}
};

extern revo_u32 revo_get_thread_id(revo_char *name);

revo_key_event_e revo_key_evt_map(revo_u32 platform_evt)
{
	revo_key_evt_map_t *map_entry_p;
	revo_u32 i;

	for(i = 0, map_entry_p = revo_key_evt_maptbl; i < REVO_KEY_EVT_MAPTBL_SIZE; i++, map_entry_p++)
	{
		if(platform_evt == map_entry_p->platform_evt)
		{
			return map_entry_p->sdk_evt;
		}
	}

	return REVO_KEY_EVT_UNDEF;
}

revo_key_value_e revo_key_val_map(revo_u32 platform_val)
{
	revo_key_val_map_t *map_entry_p;
	revo_u32 i;

	for(i = 0, map_entry_p = revo_key_val_maptbl; i < REVO_KEY_VAL_MAPTBL_SIZE; i++, map_entry_p++)
	{
		if(platform_val == map_entry_p->platform_val)
		{
			return map_entry_p->sdk_val;
		}
	}

	return REVO_KEY_MAX_VALUE;	
}

void revo_keypad_input(unsigned int platform_evt , unsigned int  platform_val)
{
	revo_key_event_e evt;
	revo_key_value_e val;

	evt = revo_key_evt_map(platform_evt);
	val = revo_key_val_map(platform_val);

	REVO_LOGI(TAG, "%s() event = %d  value = %d", __func__, evt, val);
	
	if(evt == REVO_KEY_EVT_UNDEF || val == REVO_KEY_MAX_VALUE)
	{
		REVO_LOGW(TAG, "%s() Warning: cannot map key event or value!", __func__);
		return;
	}
	revo_task_msg_send("key_test_task", evt, &val, sizeof(revo_key_value_e));
}

static void revo_keypad_task(revo_u32 argc, void *argv)
{
	revo_key_event_e event;
	revo_key_value_e value;
	revo_u32 ret;

	while(1)
	{
		REVO_LOGI(TAG, "%s() running!", __func__);
		ret = revo_task_msg_recv("key_test_task", &event, &value, sizeof(revo_key_value_e));
		if(ret == REVO_FAIL)
		{
			REVO_LOGE(TAG, "%s() Error: fail to recv msg!", __func__);
			continue;
		}
		
		REVO_LOGI(TAG, "%s() event = %d, value = %d", __func__, event, value);
		switch(event)
		{
			case REVO_KEY_EVT_PRESS:
			{
				
				break;
			}

			case REVO_KEY_EVT_RELEASE:
			{
				
				break;
			}

			default:
				break;
		}
		
		revo_task_sleep(500);
	}
}

static void revo_keypad_task_create(void)
{
	revo_task_params_t task_params;

	task_params.task_name = REVO_KEYPAD_TASK_NAME;
	task_params.queue_name = REVO_KEYPAD_TASK_QNAME;
	task_params.entry = revo_keypad_task;
	task_params.argc = 0;
	task_params.argv = 0;
	task_params.stack_size = REVO_KEYPAD_TASK_STACK_SIZE;
	task_params.queue_num = REVO_KEYPAD_TASK_QUEUE_NUM;
	task_params.priority = REVO_KEYPAD_TASK_PRIORITY;
	task_params.preempt = REVO_TASK_PREEMPT;
	task_params.auto_start = REVO_TASK_AUTO_START;
	
	revo_task_create(&task_params);
}

void revo_keypad_open(void)
{
	revo_keypad_task_create();
}
