#include "includes.h"
void LedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init( LED_GPIO, &GPIO_InitStruct );

    LedOn();
}

void LedOn(void)
{
    GPIO_SetBits(LED_GPIO,LED_PIN);
}

void LedOff(void)
{
    GPIO_ResetBits(LED_GPIO,LED_PIN);
}

void LedBlink(void)
{
    if(GPIO_ReadOutputDataBit(LED_GPIO,LED_PIN) == 0)
    {
        GPIO_SetBits(LED_GPIO,LED_PIN);
    } else {
        GPIO_ResetBits(LED_GPIO,LED_PIN);
    }
}

