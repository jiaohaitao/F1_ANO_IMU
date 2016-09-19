// timer modul 
#include"timer.h"
#include "stm32f10x.h"
//--------------------------------------------------
//--������ void Start_Timer(void)
//--������ ��
//--���أ� �� ������TIMʱ�ӣ�ʹ��TIM��
//--˵���� ������ʱ�� 
//---------------------------------------------------
void Start_Timer(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
//---------------------------------------------------
//--������ void Stop_Tmer(void)
//--������ ��
//--���أ� �ޣ�ʧ��tim���ر�timʱ�ӣ�
//--ֹͣ��ʱ��
//----------------------------------------------------
void Stop_Tmer(void)
{
	TIM_Cmd(TIM2,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
}
//-----------------------------------------------------
//--������ void Timer_Nvic_Configuration(void)
//--������ ��
//--���أ� �� 
//--˵���� timer��nvic����
//------------------------------------------------------
void Timer_Nvic_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//--------------------------------------------------------
//--������	void Timer_Configuration(void)
//--������  ��
//--���أ�  ��
//--˵����	timer����
//----------------------------------------------------------
void Timer_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);//��λtim2
	
	TIM_TimeBaseStructure.TIM_Period=10-1; //�ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� 
	TIM_TimeBaseStructure.TIM_Prescaler=(7200-1);//ʱ��Ԥ��Ƶ�� 72M/7200=10khz->0.1ms 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//������Ƶ 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);//����жϱ�־λ
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//��������ж�
    TIM_Cmd(TIM2, ENABLE);			//ʹ��TIM2
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/    

}
//---------------------------------------------------------
//--������ void TIM2_IRQHandler(void)
//--������ ��
//--���أ� ��
//--˵���� ��Ϊtim2�жϷ���������Ҫ��ӵ�stm32f10x_it.c��
//--		���timerӦ������
//----------------------------------------------------------
// void TIM2_IRQHandler(void)
// {
// 		if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
// 	{	
// 		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
//   		 time++;
// 	}	
// }
