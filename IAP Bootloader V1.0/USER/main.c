#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "stmflash.h"
#include "stm32f4xx.h"                  // Device header
#include "iap.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验52
 串口IAP实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	u32 t,i;
	vu32 total;
	u8 key;
	u16 oldcount=0;				    //老的串口接收数据值
	u32 applenth=0;				    //接收到的app代码长度
	u8 clearflag=0; 
    

    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
		delay_ms(500);
		printf("Uart init OK!\r\n");
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM

#if 1
	while(1)
	{
		//printf("Run!\r\n");
		delay_ms(500);
		LED0=1;
		delay_ms(500);
		LED0=0;
		
		if(USART_RX_CNT)
		{
			printf("开始接收\r\n");
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
				
//				printf("*.bin=\r\n");
//				for(i=0;i<applenth;i++)
//				{
//					printf("%d",USART_RX_BUF[i]);
//				}
				
				clearflag=1;
			}else oldcount=USART_RX_CNT;			
		}
		
		if(clearflag!=0)	
		{
			printf("applenth is :%d\r\n",applenth);
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 

					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					printf("固件更新完成!\r\n");	
					printf("开始执行FLASH用户代码!!\r\n");
					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
					{	 
						iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
					}else 
					{
						printf("非FLASH应用程序,无法执行!\r\n");   
					}
					
				}else 
				{   
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
			}
			clearflag=0;//标志更新了显示,并且设置7*300ms后清除显示									 
		}
	}
#else
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0;
			t=0;
			if(clearflag)
			{
				clearflag--;
			}
		}	  	 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//WK_UP按键按下
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					printf("固件更新完成!\r\n");	
				}else 
				{   
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
			}
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示									 
		}
		if(key==KEY1_PRES)	//KEY1按下
		{
			if(applenth)
			{																	 
				printf("固件清除完成!\r\n");    
				applenth=0;
			}else 
			{
				printf("没有可以清除的固件!\r\n");
			}
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示									 
		}
		if(key==KEY2_PRES)	//KEY2按下
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	  
		}
		if(key==KEY0_PRES)	//KEY0按下
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM地址
			}else 
			{
				printf("非SRAM应用程序,无法执行!\r\n");  
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	 
		}				   
		 
	}  
#endif
}
