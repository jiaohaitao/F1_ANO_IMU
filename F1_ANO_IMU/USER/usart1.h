#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>	 //	 *itoa(),USART1_printf()�����������õ������ͷ�ļ��Ķ���


void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void USART1_Send_Data(USART_TypeDef* USARTx,uint8_t *DataBuf,uint16_t DataNum);//�������ݵ�����
#endif /* __USART1_H */
