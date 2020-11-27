#ifndef _REVO_LED_H
#define _REVO_LED_H

#include "revo_led_config.h"
#include "revo_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define REVO_FIRST_LED_TIMER_NAME    "REVO_LED_BLINK"
#define REVO_TIMER_NUM_OFFSET        9
#define LED_BLINK_BASE_TIME          500

#define LED_VOL_HIGH          1
#define LED_VOL_LOW           0


typedef enum {
	LED_ON,
	LED_OFF,
	LED_REVERSE
} revo_led_ctrl_cmd_e;


typedef enum {
	LED_R,
	LED_G,
	LED_B
} revo_led_color_e;


typedef enum {
	LIGHT_OFF,
	LIGHT_ON
} revo_led_current_status_e;

typedef enum {
	REVO_LED_R_GPIO = LED_R_GPIO,
	REVO_LED_G_GPIO = LED_G_GPIO,
	REVO_LED_B_GPIO = LED_B_GPIO
} revo_led_gpio_e;

typedef enum {
	LED_ALWAYS_ON,
	LED_ALWAYS_OFF,
	LED_ALWAYS_BLINK
} revo_led_model_options_e;

typedef struct {
	revo_led_gpio_e gpio;
	revo_led_color_e color;
	revo_led_current_status_e status;
} revo_led_desc_t;


void revo_led_init();
void revo_led_blink(revo_led_color_e color, revo_u32 ms);
void revo_led_on(revo_led_color_e color);
void revo_led_off(revo_led_color_e color);






#ifdef __cplusplus
}
#endif
#endif
