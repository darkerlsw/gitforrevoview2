#include "revo_led.h"
#include "os_api.h"
#include "revo_gpio.h"


#ifdef __cplusplus
extern "C"
{
#endif

static revo_led_desc_t leds_desc[] = {
#ifdef LED_R_GPIO
	{REVO_LED_R_GPIO, LED_R /* RED LED */, LIGHT_OFF}, 
#endif

#ifdef LED_G_GPIO
	{REVO_LED_G_GPIO, LED_G /* GREEN_LED */, LIGHT_OFF}, 
#endif

#ifdef LED_B_GPIO
	{REVO_LED_B_GPIO, LED_B /* BLUE_LED */, LIGHT_OFF}, 
#endif
};

static revo_timer_ptr g_timer_id[sizeof(leds_desc)/sizeof(revo_led_desc_t)];

static void revo_led_io_ctrl(revo_led_color_e color, revo_led_ctrl_cmd_e cmd)
{
	revo_s8 status;
	revo_u32 gpio, i;

	for(i = 0; i < sizeof(leds_desc)/sizeof(revo_led_desc_t); i++)
	{
		if(color == leds_desc[i].color)
			break;
	}

	if(i == sizeof(leds_desc)/sizeof(revo_led_desc_t))
		return;
	
	gpio = leds_desc[i].gpio;

	switch(cmd)
	{
		case LED_ON:
			revo_gpio_set_value(gpio, LED_VOL_HIGH);
			leds_desc[i].status = LIGHT_ON;
		break;
		
		case LED_OFF:
			revo_gpio_set_value(gpio, LED_VOL_LOW);
			leds_desc[i].status = LIGHT_OFF;
		break;
		
		case LED_REVERSE:
			status = leds_desc[i].status;
			if(status == LIGHT_OFF)
				revo_led_io_ctrl(color, LED_ON);
			else
				revo_led_io_ctrl(color, LED_OFF);
		break;
	}
}



static void revo_led_blink_timer_handler(revo_u32 id)
{

	revo_led_io_ctrl(leds_desc[id].color, LED_REVERSE);

	return;

}

void revo_led_init() 
{
	revo_u32 i;	
	revo_s8 timer_name[20] = REVO_FIRST_LED_TIMER_NAME;
	
	for(i = 0; i < sizeof(leds_desc)/sizeof(revo_led_desc_t); i++) 
	{	
		//LED init
		revo_gpio_set_value(leds_desc[i].gpio, LED_VOL_LOW);

		//Blink timer init
		g_timer_id[i] = revo_create_period_timer(timer_name, revo_led_blink_timer_handler, i, LED_BLINK_BASE_TIME, SCI_NO_ACTIVATE);
		
		revo_printf("[REVO]register timer name = %s, g_timer_id[i] = 0x%x", timer_name, g_timer_id[i]);
		timer_name[REVO_TIMER_NUM_OFFSET]++;
	}
}

void revo_led_blink(revo_led_color_e color, revo_u32 ms)
{
	revo_u32 i;

	for(i = 0; i < sizeof(leds_desc)/sizeof(revo_led_desc_t); i++) {
		if(color == leds_desc[i].color)
			break;
	}
	
	revo_change_timer(g_timer_id[i], revo_led_blink_timer_handler, ms);
	revo_active_timer(g_timer_id[i]);
	
	return;
}

void revo_led_on(revo_led_color_e color)
{
	revo_led_off(color);
	switch(color)
	{
		case LED_R:
			revo_led_io_ctrl(LED_R, LED_ON);
		break;
		
		case LED_G:
			revo_led_io_ctrl(LED_G, LED_ON);
		break;

		case LED_B:
			revo_led_io_ctrl(LED_B, LED_ON);
		break;
	}
}

void revo_led_off(revo_led_color_e color)
{
	revo_u32 i;

	for(i = 0; i < sizeof(leds_desc)/sizeof(revo_led_desc_t); i++)
	{
		if(color == leds_desc[i].color)
			break;
	}

	if(i == sizeof(leds_desc)/sizeof(revo_led_desc_t))
		return;
	
	if(revo_is_timer_active(g_timer_id[i]))
		revo_deactive_timer(g_timer_id[i]);
	
	revo_led_io_ctrl(color, LED_OFF);

	return;
}


#ifdef __cplusplus
}
#endif