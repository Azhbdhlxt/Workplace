/*********************************************************************************
* 【编写时间】： 2019年5月21日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ (直销店)
* 【实验平台】： QX-MCS51 单片机开发板 & QX-A51智能小车
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio4
* 【程序功能】： QX-MCS51 单片机开发板DA输出		   			            			    
* 【使用说明】： 请把LED-GND上的跳线帽拔下来接到LED-DOUT上
* 【注意事项】： 
**********************************************************************************/
#include <reg52.h>
sfr P4 = 0xe8;  /*C语言中声明P4口特殊功能寄存器地址*/
sbit DAC_DATA = P4^4;//DA数据口
unsigned char DAC_VAL;//占空比参数 模拟8位DA输出，取值范围0-255
unsigned char pwm_t;//周期

#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ
typedef unsigned int INT16U;

/*====================================
函数名	：void delay(INT16U ms)
参数	：ms，毫秒延时形参
返回值	：无
描述	：12T 51单片机自适应主时钟毫秒级延时函数
====================================*/
void delay(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}
//定时器0中断
void timer0() interrupt 1
{
	pwm_t++;//周期自加到255溢出后自动归零
	if(pwm_t <= DAC_VAL)// 设置占空比
		DAC_DATA = 1;
	else
		DAC_DATA = 0;			 
}
void main()
{
	bit flag = 0;
	TMOD |= 0x02;//8位自动重装模块
	TH0 = 220;
	TL0 = 220;//11.0592M晶振下占空比最大比值是256,输出100HZ
	TR0 = 1;//启动定时器0
	ET0 = 1;//允许定时器0中断
	EA	= 1;//总中断允许
//	DAC_VAL = 169; //输出3.3V (一个数字量理论表示输出0.01953125V)
	DAC_VAL = 80;//LED10灯点亮最低电压
	while(1)
	{
		delay(5);
		if(DAC_VAL == 255)
			   flag = 1;
			   else if(DAC_VAL == 80)
					   flag = 0;
		if((DAC_VAL < 255) && (flag == 0))
			DAC_VAL++;
			else if((DAC_VAL > 80) && (flag == 1))
					 DAC_VAL--;
				
	}
}