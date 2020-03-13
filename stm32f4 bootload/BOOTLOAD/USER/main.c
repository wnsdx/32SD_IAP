#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "ftl.h" 
#include "iap.h"
#include "fattester.h"

//#include "stm32f4xx_hal_gpio.h"

FIL f_anyfile;
u8*updatebuf;//数据缓冲区，大小960K     
int main(void)
{
 	u32 free;
	vu32 total;
	int i;
	u8 t=0;	
	u8 res=0;
	u8 exfret=0;
	int SIZEOFBIN;
	FRESULT fret=FR_OK;

	TCHAR* temp_buff;
	UINT len;
	
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
	delay_ms(500);
	printf("Uart init OK!\r\n");
    LED_Init();                     //初始化LED 
    //LED1_OFF;
    KEY_Init();                     //初始化按键
    delay_ms(500);
		//LED1_ON;
  	SDRAM_Init();                   //SDRAM初始化
	W25QXX_Init();				    //初始化W25Q256
 	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池 
 	exfret=exfuns_init();				//为fatfs相关变量申请内存  
 	printf("exfret=%d\r\n",exfret);
	
 	fret = f_mount(fs[0],"0:",1); 		//挂载SD卡
 	if(fret==FR_OK)printf("fs[0] mount ok!\r\n");
	else printf("fs[0] mount error,err_code is:%d\r\n",fret);
	
	fret = f_mount(fs[1],"1:",1); 	    //挂载FLASH
	if(fret==FR_OK)printf("fs[1] mount ok!\r\n");
	else printf("fs[1] mount error,err_code is:%d\r\n",fret);
	
	if(fret==0X0D)////FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		printf("fs[1] mount error,err_code is:%d\r\n",fret);
		fret=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:");	//设置Flash磁盘的名字为：1:
		}else{
			printf("fs[1] mkfs_err,err_code is:%d\r\n",fret);
		}
		delay_ms(1000);
	}

	fret = f_getcwd(temp_buff,len);
	printf("current path:%s\r\n",temp_buff);
	
	mf_scan_files("0:");
	
	updatebuf=mymalloc(SRAMEX,960*1024);//180	
	res=f_open(&f_anyfile,"0:/1.bin",FA_READ);//打开文件
	
	SIZEOFBIN=f_size(&f_anyfile);
	
	if(res==FR_OK)
	{
		res=f_read(&f_anyfile,updatebuf,SIZEOFBIN,(UINT*)br);

		delay_ms(1500);
		total=((*(vu32*)(0X20001000+4))&0xFF000000);
		printf("total=%x\r\n",total);
		if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
		{
			
			for(i=0;i<SIZEOFBIN;i++)
			{
				if(0==i)printf("SIZEOFBIN is %d\r\n",SIZEOFBIN);
				printf("%d",updatebuf[i]);
				if(SIZEOFBIN-1==i)printf("\r\n");
			}
			
			printf("开始更新!\r\n");
			iap_write_appbin(FLASH_APP1_ADDR,updatebuf,SIZEOFBIN);//更新FLASH代码   	
			printf("更新完成!\r\n");		
			//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
			printf("跳转\r\n");	
			//LED1_OFF;
			iap_load_app(FLASH_APP1_ADDR);
			printf("跳转完成\r\n");
		}else 
		{   
			printf("非FLASH应用程序!\r\n");
		}
	}
	else
  {
	 printf("无数据\r\n");
	 printf("error\r\n");
	}
	
	while(1)
	{
		//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);
		LED0=0;
		//LED1_ON
		delay_ms(500);
		LED0=1;
		delay_ms(500);
		//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
		//printf("跳转完成\r\n");
	}

}
