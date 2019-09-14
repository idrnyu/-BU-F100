#include "bluetooth.h"

// char *AT_CMD  AT�����ַ�
// char *resolveTXT  �뷵�ص��ַ�
// uint8_t expectFlat  ��������ֵ
uint8_t AT_Bluetooth_config(uint8_t *AT_CMD, char *resolveTXT, uint8_t expectFlat)
{
  USART_OUT(USART2, AT_CMD); // ����AT����
  Delay_ms(1);   // ��ʱ��������һ������
  USART_OUT(USART1, AT_CMD); // ����AT�������
  while (!RX2_Flat);  // �յ����ڿ��б�־
	Delay_ms(10);  // ��ʱ��ȴ������ȶ�
  // int ch = indexOf((char *)RX2_Buffer, resolveTXT);  // �жϽ��յ��ַ����Ƿ����ĳ���ַ���
  // if (ch != -1)  // �������
  char *ch = strstr((char *)RX2_Buffer, resolveTXT);  // �жϽ��յ��ַ����Ƿ����ĳ���ַ���
  if (ch != NULL)  // �������
  {
    GPIO_SetBits(LED_GPIO_x, LED_Pin_x);
		Delay_ms(50);   // ��ʱ��������ձ�־     ��Ҫ��ʱ����  ��Ȼ��һ�ν���û���ж�  �ַ�������
		RX2_Flat = false;
		memset(RX2_Buffer, 0, 1024); //�������
    return expectFlat;  // ����ָ�����
  }
	Delay_ms(50);   // ��ʱ��������ձ�־   ��Ȼ��һ�ν���û���ж�  �ַ������� 
	RX2_Flat = false;
  memset(RX2_Buffer, 0, 1024); //�������
  return 0;
}
