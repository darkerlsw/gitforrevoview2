#ifndef _REVO_GPIO_H
#define _REVO_GPIO_H

#include "revo_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
*@brief        This function used to set the value of a gpio pin
*
*@param[in]    gpio_id:  gpio id
*@param[in]    b_on:     Logical 0 or 1
*
*@return       N/A
*/
void revo_gpio_set_value(revo_u32 gpio_id, revo_u8 b_on);


#ifdef __cplusplus
}
#endif

#endif