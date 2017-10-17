#ifndef __KEY_H
#define __KEY_H

#define KEY_GPIO    GPIOC
#define KEY_PIN     GPIO_Pin_13
void KeyInit(void);
uint8_t GetKeyValue(void);

#endif
