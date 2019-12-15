#include "hx711.h"
static uint32_t zero = 0;
int32_t reading = 0;
int32_t old_reading = 0;
uint32_t reading1;
static TIM_TypeDef *timer;
float gram = 151962.0f;

void DWT_Delay_us(volatile uint32_t microseconds)
{
 uint32_t clk_cycle_start = DWT->CYCCNT;
 /* Go to number of cycles for system */
 microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 /* Delay till end */
 while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

void hx711_init(TIM_TypeDef *t) {
    //hx711_gpio_init();
    timer = t;

    // perform calibration
    zero = 0;

    // dummy read to set gain
    hx711_single_read('a');
    HAL_Delay(1000);

    zero = hx711_avg_read(16, 'a');
}

int32_t hx711_single_read(char channel) {
    uint8_t i;
//    int32_t reading = 0;
//    int32_t old_reading = 0;
    int dem;
    old_reading = reading;
    reading = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
    switch (channel)
    {
    case 'a':
    	dem = 25;
    	break;
    case 'b':
    	dem = 26;
    	break;
    }
    for (i = 0; i < 25; ++i) {
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1); //CLK
    	DWT_Delay_us(1);
        reading = reading << 1;
        uint8_t bit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12); //Data
        if (bit)
            reading++;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
        DWT_Delay_us(1);
    }
    reading = reading << (32 - i);
    reading = (reading * 10 + old_reading*90)/100;
    reading1 = (uint32_t)reading;
    return reading;
}

int32_t hx711_avg_read(uint8_t samples, char channel) {
    int64_t adc_data = 0;
    uint8_t i;

    for (i = 0; i < samples; ++i) {
        adc_data += hx711_single_read(channel);
        HAL_Delay(10);
    }
    adc_data /= samples;
    return adc_data;
}

float hx711_read_gram(char channel) {
    int32_t read = hx711_avg_read(8, channel);

    return (float)read / gram;
}
