
#include"usart1.h"
#include <stdarg.h>	 //	 *itoa(),USART1_printf()�����������õ������ͷ�ļ��Ķ��� ��Ҫ���ڽ��c
//�����У�������������ȷ�������

//============================================================================================
//���������ͻ����������ݵ�����
//������usartx�����ں� DataBbuf����������ַ DataNum��Ҫ�������ݵĳ���
//����ֵ��No
//============================================================================================
void USART1_Send_Data(USART_TypeDef* USARTx,uint8_t *DataBuf,uint16_t DataNum)//�������� ���������ݵ�����
{	
	uint16_t i=0;
	while(i<DataNum)
	{
		USART_SendData(USARTx,DataBuf[i]);
		i++;
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );		
	}
}
//============================================================================================
//--����  :	int fputc(int ch,FILE *f)	 
//--˵��  : �������Ǳ�������c�ṩ��printf�����������õģ���51��Ƭ����puts��printf��������
//			��������stm32����Ҫ��Щ�޸ģ������ض��򣬵��û��Լ���дc�⺯��ʱ�����������õ���⺯��ͬ����
//			�û�����ʱ����Ĭ�ϵ����û���д�ĺ������������ǾͿ����ض���⺯����
//			��printf�����ǵ���fputc����������������ģ��������Ǹ���stm32���ص��޸���fputc���
//			����printf�ʺ�stm32�Ĵ����ˡ�������ض���
//--����  :   ch f
//--����ֵ:   ch 
//--��Χ  : ��c�⺯��printf����
//--ʱ��  ��2012��9��22��21:22:42
//--��������ӡ�stdio.h�����c�ı�׼�������ͷ�ļ����������fputc���ǰ�������
//--��λ�Ҫ���target/use microLib��ʹ�ñ������ṩ��c΢�⣬����ʹstdio������ʹ��printf�������ض���fputc
//============================================================================================
int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(unsigned char) ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	return (ch);
}
//============================================================================================
//--����  :	void USART1_Config(void)	 
//--˵��  : ��Ҫ���ڳ�ʼ��USART1,����:115200-8-N-1	, ���Ծ�������޸�
//--����  :   �� 
//--����ֵ:   ��  
//--��Χ  : ����Χ���ã���ʼ��usart1 
//--ʱ��  ��2012��9��21��20:14:48
//============================================================================================


void USART1_Config(void)
{
	//gpio��usart��ʼ���ṹ�嶨��
	//��gpio����ʱ����������ʱ������Ҫ��ʼ��ͬʱgpio��ppp���裬��������ʹ�ܺͿ���ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//����gpio������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

	//gpio��ʼ���ṹ���Ա��ֵ
	//��ʼ��PA��9�������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ִ�������gpio��ʼ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA.10��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART��ʼ��
	//���������ã����ÿ⺯�������ǿ���ֱ���������ò����ʣ�������Ҫ���м���USARTDIV�ķ�Ƶ����
	USART_InitStructure.USART_BaudRate = 115200;
	
	//���ô��ڴ�����ֳ��������̰�������Ϊ��õ�8λ�ֳ���Ҳ��������Ϊ9λ��
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	//����ֹͣλ����ͨѶЭ���е�ֹͣλ����Ϊ1λ��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	//������żУ��λ�������̲�������żУ��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	
	//����Ӳ�������ơ�������Ӳ������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	//���ô��ڵ�ģʽ��Ϊ������˫��ȫ˫��ͨѶ����Ҫ��Rx��Txģʽ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	//ִ������Ĳ���
	USART_Init(USART1, &USART_InitStructure);

	//ע��.�����ж�ʹ�ܣ�����Ҫ��(��?���� �뵱����������Ϊ������࣬������һ�����һ���ϵ�ʱ��
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//����USART_Cmd() ʹ��USART1���衣��ʹ������ʱ������Ҫʹ����ʱ�ӣ���Ҫ���ô˺���ʹ������ſ�������ʹ�á�
	USART_Cmd(USART1, ENABLE);
}
//============================================================================================
//--����  :	static char *itoa(int value, char *string, int radix)	
//--˵��  : ��Ҫ���ڳ�ʼ��USART1,����:115200-8-N-1	, ���Ծ�������޸�
//--����  :   value : Ҫת�������������磺1��2��3
//			  string: ת������ַ���
//			  radix : radix =10 ��ʾ10���ƣ��������Ϊ0
//--����ֵ:   	return string;  
//--��Χ  : ������USART1_printf()���ã�������ת�����ַ��� ,���ҷ����ַ���
//============================================================================================
static char *itoa(int value, char *string, int radix)	
//˵���˺���Ϊ�ڲ�������static��������ֵΪָ���ͣ�*�����ַ�����
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}
	
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    /* Make the value positive. */
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	/* Null terminate the string. */
	*ptr = 0;
	
	return string;

} /* NCL_Itoa */
//============================================================================================
//--����  :	void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)	
//--˵��  : ���ڴ�ӡ������谴��ָ���ĸ�ʽ
//--����  :  USARTx ����ͨ��������ֻ�õ��˴���1����USART1
//			  Data   Ҫ���͵����ڵ����ݵ�ָ��
//			  -...    ��������
//--����ֵ:   	�� 
//--��Χ  : ���ڴ��������ָ����ʽ������
//			����ַ�������1����USART1_printf( USART1, "\r\n this is a demo \r\n" );
//            		 	��2����USART1_printf( USART1, "\r\n %s \r\n","jiaohaitao" );  //��ʵ�Ӳ���%s�����ʽ������ν
//							 ���Ը�������Ĵ���֪������%s�Ļ������ַ���ʽ��������ӵĻ�Ĭ���ַ������
//						��3����USART1_printf( USART1, "\r\n %d %d\r\n", i,10 );�ڡ�������Ӹ�ʽ������Ӧ�����������������ֵ��
//						 (4) : USART1_printf( USART1, "\r\n %d\r\n",10 );
//            		 
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)  //Ϊʲô���������������������ⲻ֪��
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);	   //�Ҹо���������������ǰ��������е���һ������ϵ�д���������������
	 //��������ã���USART1��printf���������������е�һ�������еĲ�������Data�������ڵ���ʱ�����Ǻ���Ĳ�����һ�����ơ�
	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
	{									  
	switch ( *++Data )
	{
		case 'r':							          //�س���
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							          //���з�
			USART_SendData(USARTx, 0x0a);	
			Data ++;
		break;
		
		default:
			Data ++;
		break;
	}			 
	}
	else if ( *Data == '%')
	{									  //
	switch ( *++Data )
	{				
		case 's':										  //�ַ���
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	case 'd':										//ʮ����
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}		 
	} /* end of else if */
	else USART_SendData(USARTx, *Data);		//++ ��* ���ȼ���++���ȼ����ߡ����Բ��ܣ�*Data++��=��*��Data++����
	//�����Ļ��� �ͻ��õ�һ���ַ�������
	Data++;
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
//============================================================================================
//--����  :	void NVIC_Configuration(void)	 
//--˵��  : ��Ҫ���ڳ�ʼ��NVIC,���ڽ����ж�����
//--����  :   �� 
//--����ֵ:   ��  
//--��Χ  : ����Χ���ã���ʼ��NVIC.
//--ʱ��  ��2012��9��21��20:22:58
//--˵��  ����Ҫ��it.c�ļ��Լ���ӣ��жϷ�����.
//============================================================================================
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//����ĺ����Ƕ�Ӧ������жϷ����������Լ�copy��it.c�ļ��¡�
/*
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART1->DR;
	  	printf("%c",c);    //�����ܵ�������ֱ�ӷ��ش�ӡ
		USART1_printf(USART1," %d ",c);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3,
		(BitAction)((1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3))));
	} 
	 
} */

