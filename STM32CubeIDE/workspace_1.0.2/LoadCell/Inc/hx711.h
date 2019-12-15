#ifndef __HX711_H__
#define __HX711_H__

#include "math.h"

#include "stm32f1xx_hal.h"
#include "main.h"

//#include "common_lib/utils.h"
void DWT_Delay_us(volatile uint32_t microseconds);
void hx711_init(TIM_TypeDef *timer);
int32_t hx711_single_read(char channel);
int32_t hx711_avg_read(uint8_t samples, char channel);
float hx711_read_gram(char channel);

#endif // __HX711_H__
