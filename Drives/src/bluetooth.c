#include "bluetooth.h"

// char *AT_CMD  AT命令字符
// char *resolveTXT  想返回的字符
// uint8_t expectFlat  期望返回值
uint8_t AT_Bluetooth_config(uint8_t *AT_CMD, char *resolveTXT, uint8_t expectFlat)
{
  USART_OUT(USART2, AT_CMD); // 进入AT命令
  Delay_ms(1);   // 延时发送下面一个命令
  USART_OUT(USART1, AT_CMD); // 进入AT命令回显
  while (!RX2_Flat);  // 收到串口空闲标志
	Delay_ms(10);  // 延时后等待接收稳定
  // int ch = indexOf((char *)RX2_Buffer, resolveTXT);  // 判断接收的字符串是否包含某个字符串
  // if (ch != -1)  // 如果包含
  char *ch = strstr((char *)RX2_Buffer, resolveTXT);  // 判断接收的字符串是否包含某个字符串
  if (ch != NULL)  // 如果包含
  {
    GPIO_SetBits(LED_GPIO_x, LED_Pin_x);
		Delay_ms(50);   // 延时后清楚接收标志     需要延时处理，  不然下一次进来没法判断  字符串包含
		RX2_Flat = false;
		memset(RX2_Buffer, 0, 1024); //清空数组
    return expectFlat;  // 返回指定结果
  }
	Delay_ms(50);   // 延时后清楚接收标志   不然下一次进来没法判断  字符串包含 
	RX2_Flat = false;
  memset(RX2_Buffer, 0, 1024); //清空数组
  return 0;
}
