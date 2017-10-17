#ifndef __LED_H
#define __LED_H

#define LED_GPIO    GPIOA
#define LED_PIN     GPIO_Pin_14
void LedInit(void);
void LedOn(void);
void LedOff(void);
void LedBlink(void);
#endif
