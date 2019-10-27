#include "stm32f10x.h"
#include "mystr.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "bluetooth.h"


uint8_t RX1_Buffer[1024];  // 接收缓冲区  橙色  绿色  蓝色  紫色
uint8_t RX2_Buffer[1024];  // 接收缓冲区
uint8_t RX3_Buffer[1024];  // 接收缓冲区

bool RX1_Flat = false;
bool RX2_Flat = false;
bool RX3_Flat = false;

uint8_t atRunFlat = 1;

int main(void)
{
	Delay_Init();
	Init_LED();
	USART_Config(USART1, 115200, false, false);
	USART_Config(USART2, 115200, true, true);
	
	for(u8 i=0; i<6; i++)
	{
		Delay_ms(1000);
	}
	
	atRunFlat = AT_Bluetooth_config("+++\r\n", "AT ON", 2);       // 尝试进入AT命令  成功=2  否则=0
	if(atRunFlat == 0)																						// 进入AT命令失败
	{
		atRunFlat = AT_Bluetooth_config("ATCN\r\n", "ATCN OK", 1);  // 需要先关闭AT模式  关闭成功 1 否则 = 0；
	}
	if(atRunFlat == 1)                                            // 关闭AT模式成功
	{
		atRunFlat = AT_Bluetooth_config("+++\r\n", "AT ON", 2);			// 重新进入AT命令  成功=2  否则=0
	}
	if(atRunFlat == 0){																						// 关闭失败 或者 重新进入失败  异常处理
		while(1)
		{
			GPIO_SetBits(LED_GPIO_x, LED_Pin_x);
			Delay_ms(80);
			GPIO_ResetBits(LED_GPIO_x, LED_Pin_x);
			Delay_ms(1000);
		}
	}
	if(atRunFlat == 2)		//	成功进入AT
	{
		atRunFlat = AT_Bluetooth_config("ATCN\r\n", "ATCN OK", 200);  // 退出AT命令
	}


	
	while (1)
	{
		GPIO_SetBits(LED_GPIO_x, LED_Pin_x);
		Delay_ms(50);
		GPIO_ResetBits(LED_GPIO_x, LED_Pin_x);
		Delay_ms(10);
		
		if(strstr((char *)RX2_Buffer, "TTM:CONNECTED") != NULL)  // 用户已连接
		{
			RX2_Flat = false;
			memset(RX2_Buffer, 0, 1024); //清空数组
		}
		if(strstr((char *)RX2_Buffer, "TTM:CONNECTED") != NULL)  // 用户已断开链接
		{
			RX2_Flat = false;
			memset(RX2_Buffer, 0, 1024); //清空数组
		}
		
		if(RX2_Flat)
		{
			// char ch4[] = { 0x01, 0x07, 0x16, 0x01, 0x01, 0x4E, 0x55, 0x4C, 0x4C };
			// 81           01 07 16 01 01 4E 55 4C 4C        5B 01 FA


			// char arr5[] = { 0x81, 0x01, 0x0A, 0x16, 0x01, 0x01, 0x4E, 0x55, 0x4C, 0x4C, 0x31, 0x32, 0x33, 0xF4, 0x01, 0xFA };
			// char ch5[] = { 0x01, 0x0A, 0x16, 0x01, 0x01, 0x4E, 0x55, 0x4C, 0x4C, 0x31, 0x32, 0x33 };
			// 81           01 0A 16 01 01 4E 55 4C 4C 31 32 33         F4 01 FA
			RX2_Flat = false;
			memset(RX2_Buffer, 0, 1024); //清空数组
		}
		// USART_OUT(USART1, "我的电脑 \r\n");
		// USART_OUT(USART1, "字符串是：%s","abcdefg \r\n");
		// char arr[] = { 0xFF, 0xFB, 0xAA };
		// USART_OUT(USART1, "%h", arr);
	}
}

