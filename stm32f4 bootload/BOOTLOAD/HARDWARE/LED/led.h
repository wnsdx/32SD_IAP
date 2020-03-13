#ifndef _LED_H
#define _LED_H
#include "sys.h"
#include "stm32f4xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define LED1_PIN             GPIO_Pin_12        		 // LED1 引脚      
#define LED1_PORT            GPIOD                  // LED1 GPIO端口     
#define LED1_CLK             RCC_AHB1Periph_GPIOD	 // LED1 GPIO端口时钟

#define LED0 PDout(12)   //LED0
#define LED1 PGout(7)   //LED1

#define LED1_ON 	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);//GPIO_ResetBits(LED1_PORT,LED1_PIN);	// 输出低电平，点亮LED1	
#define LED1_OFF 	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);//GPIO_SetBits(LED1_PORT,LED1_PIN);		// 输出高电平，关闭LED1	

//#define LED1_ON 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	// 输出低电平，点亮LED1	
//#define LED1_OFF 	  GPIO_SetBits(LED1_PORT,LED1_PIN);		// 输出高电平，关闭LED1	

void LED_Init(void);
#endif
