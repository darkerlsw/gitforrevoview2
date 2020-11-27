#include "revo_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*gpio*/
void revo_gpio_set_value(revo_u32 gpio_id, revo_u8 b_on) {
	GPIO_SetValue(gpio_id, b_on);
}


#ifdef __cplusplus
}
#endif