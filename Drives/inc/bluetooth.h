#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"
#include "string.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include <stdbool.h>

#undef NULL
#define NULL 0

extern uint8_t RX1_Buffer[];  // ���ջ�����
extern uint8_t RX2_Buffer[];  // ���ջ�����
extern uint8_t RX3_Buffer[];  // ���ջ�����

extern _Bool RX1_Flat;  // ���ձ�־
extern _Bool RX2_Flat;  // ���ձ�־
extern _Bool RX3_Flat;  // ���ձ�־

uint8_t AT_Bluetooth_config(uint8_t *AT_CMD, char *resolveTXT, uint8_t expectFlat);

#endif /* __BLUETOOTH_H */


