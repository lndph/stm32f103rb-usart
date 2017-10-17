#include "includes.h"
//printf使用串口
#define USING_USART_PRIINTF     USART1
void Usart1Init(void)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );
	/* Configure USART1 Rx (PA10) as input floating */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStruct );

	/* Configure USART1 Tx (PA9) as alternate function push-pull */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOA, &GPIO_InitStruct );

	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = UART1_BAUD;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No ;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStruct);

    /* Enable the USARTx Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

extern void USART1_IRQHandler(void)
{
    volatile uint8_t ch;
	//接收到数据USART_ GetITStatus
	if (USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		ch = USART_ReceiveData(USART1);
	}
	//发送数据
	if (USART_GetITStatus(USART1,USART_IT_TXE))
	{
        USART_ClearFlag(USART1, USART_FLAG_TXE);
	}
}

void Usart2Init(void)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
	/* Configure USART1 Rx (PA10) as input floating */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStruct );

	/* Configure USART1 Tx (PA9) as alternate function push-pull */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOA, &GPIO_InitStruct );

	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = UART2_BAUD;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No ;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStruct);

	/* Enable the USARTx Interrupt */
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}

extern void USART2_IRQHandler(void)
{
    volatile uint8_t ch;
	//接收到数据USART_ GetITStatus
	if (USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		ch = USART_ReceiveData(USART2);
	}
	//发送数据
	if (USART_GetITStatus(USART2,USART_IT_TXE))
	{
        USART_ClearFlag(USART2, USART_FLAG_TXE);
	}
}

void UsartInit(void)
{
    Usart1Init();

    Usart2Init();
}

void uart_putbuf(USART_TypeDef* USARTx, uint8_t * buf, uint16_t len)
{
    while (len--) {
        USART_SendData(USARTx, *buf++);

        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {
            ;
        }
    }
}

void uart_putchar(USART_TypeDef* USARTx, uint8_t ch)
{
    USART_SendData(USARTx, ch);

    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {
        ;
    }
}

struct __FILE
{
	int handle;
};
FILE __stdin, __stdout, __stderr;
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口 */
    USART_SendData(USING_USART_PRIINTF, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USING_USART_PRIINTF, USART_FLAG_TC) == RESET);

    return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    /* 等待串口输入数据 */
    while (USART_GetFlagStatus(USING_USART_PRIINTF, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USING_USART_PRIINTF);
}

