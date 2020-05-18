


#include "hardware_version.h"
#include "stm32f4xx_hal.h"


#if 0
#define   HW0   //PA5
#define   HW1   //PA6
#define   HW2   //PA7
#define   HW3   //PC4
#define   HW4   //PC5
#endif



void init_hardware_version(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}




uint32_t get_hardware_version(void)
{
    uint8_t h0,h1,h2,h3,h4;

    h0 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)?1:0;
    h1 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)?1:0;
    h2 = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)?1:0;
    h3 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)?1:0;
    h4 = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)?1:0;


    
    return (h4<<4|h3<<3|h2<<2|h1<<1|h0<<0)&0x000000ff;

}







