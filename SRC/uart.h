#ifndef __UART_H
#define __UART_H

#define UART1_BAUD  115200
#define UART2_BAUD  115200

void UsartInit(void);
void uart_putbuf(USART_TypeDef* USARTx, uint8_t * buf, uint16_t len);
void uart_putchar(USART_TypeDef* USARTx, uint8_t ch);

#endif
