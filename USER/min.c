#include "stm32f10x.h"
#include "mystr.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "bluetooth.h"


uint8_t RX1_Buffer[1024];  // ���ջ�����  ��ɫ  ��ɫ  ��ɫ  ��ɫ
uint8_t RX2_Buffer[1024];  // ���ջ�����
uint8_t RX3_Buffer[1024];  // ���ջ�����

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
	
	atRunFlat = AT_Bluetooth_config("+++\r\n", "AT ON", 2);       // ���Խ���AT����  �ɹ�=2  ����=0
	if(atRunFlat == 0)																						// ����AT����ʧ��
	{
		atRunFlat = AT_Bluetooth_config("ATCN\r\n", "ATCN OK", 1);  // ��Ҫ�ȹر�ATģʽ  �رճɹ� 1 ���� = 0��
	}
	if(atRunFlat == 1)                                            // �ر�ATģʽ�ɹ�
	{
		atRunFlat = AT_Bluetooth_config("+++\r\n", "AT ON", 2);			// ���½���AT����  �ɹ�=2  ����=0
	}
	if(atRunFlat == 0){																						// �ر�ʧ�� ���� ���½���ʧ��  �쳣����
		while(1)
		{
			GPIO_SetBits(LED_GPIO_x, LED_Pin_x);
			Delay_ms(80);
			GPIO_ResetBits(LED_GPIO_x, LED_Pin_x);
			Delay_ms(1000);
		}
	}
	if(atRunFlat == 2)		//	�ɹ�����AT
	{
		atRunFlat = AT_Bluetooth_config("ATCN\r\n", "ATCN OK", 200);  // �˳�AT����
	}


	
	while (1)
	{
		GPIO_SetBits(LED_GPIO_x, LED_Pin_x);
		Delay_ms(50);
		GPIO_ResetBits(LED_GPIO_x, LED_Pin_x);
		Delay_ms(10);
		
		if(strstr((char *)RX2_Buffer, "TTM:CONNECTED") != NULL)  // �û�������
		{
			RX2_Flat = false;
			memset(RX2_Buffer, 0, 1024); //�������
		}
		if(strstr((char *)RX2_Buffer, "TTM:CONNECTED") != NULL)  // �û��ѶϿ�����
		{
			RX2_Flat = false;
			memset(RX2_Buffer, 0, 1024); //�������
		}
		
		if(RX2_Flat)
		{
			// char ch4[] = { 0x01, 0x07, 0x16, 0x01, 0x01, 0x4E, 0x55, 0x4C, 0x4C };
			// 81           01 07 16 01 01 4E 55 4C 4C        5B 01 FA


			// char arr5[] = { 0x81, 0x01, 0x0A, 0x16, 0x01, 0x01, 0x4E, 0x55, 0x4C, 0x4C, 0x31, 0x32, 0x33, 0xF4, 0x01, 0xFA };
			// char ch5[] = { 0x01, 0x0A, 0x16, 0x01, 0x01, 0x4E, 0x55, 0x4C, 0x4C, 0x31, 0x32, 0x33 };
			// 81           01 0A 16 01 01 4E 55 4C 4C 31 32 33         F4 01 FA
			RX2_Flat = false;
			memset(RX2_Buffer, 0, 1024); //�������
		}
		// USART_OUT(USART1, "�ҵĵ��� \r\n");
		// USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg \r\n");
		// char arr[] = { 0xFF, 0xFB, 0xAA };
		// USART_OUT(USART1, "%h", arr);
	}
}

