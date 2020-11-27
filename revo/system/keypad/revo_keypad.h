#ifndef _REVO_KEYPAD_H
#define _REVO_KEYPAD_H

#include "revo_typedef.h"
#include "revo_keypad_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TAG "keypad"

/* wrapper */
#define REVO_KEY_EVT_MAPTBL_SIZE sizeof(revo_key_evt_maptbl)/sizeof(revo_key_evt_map_t)
#define REVO_KEY_VAL_MAPTBL_SIZE sizeof(revo_key_val_maptbl)/sizeof(revo_key_val_map_t)

typedef enum {
	REVO_KEY_EVT_PRESS,    //按键按下
	REVO_KEY_EVT_PRESS_HOLD, //按键长按
	REVO_KEY_EVT_RELEASE,  //按键松开
	REVO_KEY_EVT_UNDEF     //按键事件未定义
} revo_key_event_e;

typedef enum {
	REVO_KEY_VOLUP,
	REVO_KEY_VOLDN,
	REVO_KEY_FUNC,
	REVO_KEY_MAX_VALUE
} revo_key_value_e;

typedef struct {
	revo_key_event_e sdk_evt;
	revo_u32 platform_evt;
}revo_key_evt_map_t;

typedef struct {
	revo_key_value_e sdk_val;
	revo_u32 platform_val;
}revo_key_val_map_t;

revo_key_event_e revo_key_evt_map(revo_u32 platform_evt);
revo_key_value_e revo_key_val_map(revo_u32 platform_val);
void revo_keypad_input(unsigned int platform_evt , unsigned int  platform_val);
void revo_keypad_open(void);

#ifdef __cplusplus
}
#endif
#endif
