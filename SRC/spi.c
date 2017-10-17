#include "includes.h"
#define GPIO_ADSK	GPIOA
#define GPIO_ADDO   GPIOA
#define PIN_ADSK	GPIO_Pin_5
#define PIN_ADDO 	GPIO_Pin_7

#define ADSK(x)		do{\
					if(x){GPIO_SetBits(GPIO_ADSK,PIN_ADSK);}\
					else{GPIO_ResetBits(GPIO_ADSK,PIN_ADSK);}\
					}while(0)
#define ADDO		GPIO_ReadInputDataBit(GPIO_ADDO,PIN_ADDO)

void SpiInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = PIN_ADDO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIO_ADDO,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  PIN_ADSK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIO_ADSK,&GPIO_InitStructure);
    ADSK(0);
}

uint32_t ReadCount(void)
{
	uint32_t count;
	uint8_t i;

	ADSK(0);
	count = 0;
	while(ADDO);
	for (i = 0; i < 24; i++)
	{
		ADSK(1);
		count <<= 1;
		ADSK(0);
		if(ADDO) count++;
	}
	ADSK(1);
	count = count^0x800000;
	ADSK(0);
	return (count);
}

