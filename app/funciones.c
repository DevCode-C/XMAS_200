#include "stm32f0xx.h"
#include <stdint.h>
#include "app_bsp.h"
#include "stm32f070xb.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_rcc.h"
#include "funciones.h"

void blinky(void)
{
    HAL_GPIO_TogglePin(GPIOC,0xff);
}

void altern(void)
{
    static uint8_t pin=0;
    pin ^=1; 
    HAL_GPIO_WritePin(GPIOC,0xAA,pin);
    HAL_GPIO_WritePin(GPIOC,~(0xAA),!pin);
}

void rolling(void)
{
    static uint8_t i = 128;
    HAL_GPIO_WritePin(GPIOC,i,SET);
    HAL_GPIO_WritePin(GPIOC,~(i),RESET);
    i >>= 1;
    if (i == 0)
    {
        i =128;
    }
    

}

void kit(void)
{
    const uint8_t sec[8] = {0x81,0x42,0x24,0x18,0x24,0x42,0x81};
    static uint8_t i = 0;
    HAL_GPIO_WritePin(GPIOC,sec[i],SET);
    HAL_GPIO_WritePin(GPIOC,~(sec[i]),RESET);
    i++;
    if (i>=7)
    {
        i=0;
    }
    
}

void car(void)
{
    static uint8_t i = 0;

    HAL_GPIO_WritePin(GPIOC,0xC3,SET);
    HAL_GPIO_WritePin(GPIOC,~(0xC3),RESET);
    if (i>1)
    {
        
        HAL_GPIO_WritePin(GPIOC,0x18,SET);
        HAL_GPIO_WritePin(GPIOC,~(0x18),RESET);
        i=0;
    }
    i++;
}

void none(void)
{
    HAL_GPIO_WritePin(GPIOC,0xFF,RESET);
    
}

