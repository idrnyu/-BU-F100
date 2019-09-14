#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "led.h"
#include "dma.h"
#include <string.h>
#include <stdbool.h>


extern uint8_t RX1_Buffer[];  // ���ջ�����
extern uint8_t RX2_Buffer[];  // ���ջ�����
extern uint8_t RX3_Buffer[];  // ���ջ�����

extern _Bool RX1_Flat;  // ���ձ�־
extern _Bool RX2_Flat;  // ���ձ�־
extern _Bool RX3_Flat;  // ���ձ�־

extern uint8_t atRunFlat;  // at���б�־
int chLength(const char *ch);
void NVIC_Configuration(uint8_t USARTx_IRQn);
char *itoa(int value, char *string, int radix);
void USART_GPIO_Config(GPIO_TypeDef* GPIOx, uint16_t TX_Pin_x, uint16_t RX_Pin_x);
void USART_Config(USART_TypeDef *USARTx, uint32_t BaudRate, bool isIRQ, bool isRxDMA);
void USART_OUT(USART_TypeDef *USARTx, uint8_t *Data, ...);

void USART_OUT_Arr(USART_TypeDef* USARTx, uint8_t* Data);

#endif  /* __USART_H */

