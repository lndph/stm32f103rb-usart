#include "includes.h"

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Configure USART1 Rx (PA10) as input floating */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( KEY_GPIO, &GPIO_InitStruct );
}

uint8_t GetKeyValue(void)
{
    return GPIO_ReadInputDataBit( KEY_GPIO, GPIO_Pin_13 );
}

