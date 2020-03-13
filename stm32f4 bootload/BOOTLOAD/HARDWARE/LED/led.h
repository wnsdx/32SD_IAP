#ifndef _LED_H
#define _LED_H
#include "sys.h"
#include "stm32f4xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define LED1_PIN             GPIO_Pin_12        		 // LED1 ����      
#define LED1_PORT            GPIOD                  // LED1 GPIO�˿�     
#define LED1_CLK             RCC_AHB1Periph_GPIOD	 // LED1 GPIO�˿�ʱ��

#define LED0 PDout(12)   //LED0
#define LED1 PGout(7)   //LED1

#define LED1_ON 	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);//GPIO_ResetBits(LED1_PORT,LED1_PIN);	// ����͵�ƽ������LED1	
#define LED1_OFF 	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);//GPIO_SetBits(LED1_PORT,LED1_PIN);		// ����ߵ�ƽ���ر�LED1	

//#define LED1_ON 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	// ����͵�ƽ������LED1	
//#define LED1_OFF 	  GPIO_SetBits(LED1_PORT,LED1_PIN);		// ����ߵ�ƽ���ر�LED1	

void LED_Init(void);
#endif
