/*********************************************************************************
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio4	
* 【程序功能】： 电子万年历,LCD1602液晶显示，显示时分秒、星期、公历、阴历、闹钟、环境温度、可以校时和设置闹钟。			   			            			    
* 【使用说明】： 使用3条杜邦线把P10接到DS1302的TSCLK上，P11接到TIO，P12接到TRST上。
				 接上LCD1602液晶。
* 【按键功能说明】：在正常显示状态下S2按键为切换公历、时钟/阴历、闹钟显示，在设置状态下S2为设置左移。
					S5为设置键，在显示公历时钟状态下，按下S5为设置公历、时钟，再次按下S5为写入设置参数并退出
					在显示阴历、闹钟状态下，按下S5为设置阴历、闹钟，再次按下S5为写入设置参数并退出
					在设置状态下，S3为数据加
					在设置状态下，S4为数据减
*  说明：免费开源，不提供源代码分析.
**********************************************************************************/
#define  _MAIN_C
#include "config.h"
#include "LCD1602.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "app.h"
#include "Key_Buzzer.h"
#include "main.h"

bit flag1s = 0;    //1s定时标志位
bit flag200ms = 0; //200ms定时标志
enum eStaSystem staSystem = E_NORMAL;  //系统运行状态

void main()
{
	EA = 1;			//开总中断
	Start18B20();   //启动温度转换
	Init_Timer0();  //初始化定时器0
	Init_LCD1602(); //LCD1602初始化
	Init_DS1302();	//初始化DS1302
	while (!flag1s);//上电后延时1秒
    flag1s = 0;
	RefreshTime();	//刷新时间
	RefreshDate(1);	//立即刷新日期
	RefreshTemp(1); //立即刷新温度显示

	while(1)
	{
	    KeyDriver();	//按键检测
		if(flag200ms)	//200ms时间到
		{
			flag200ms = 0;	//清零标志位
			AlarmMonitor(); //监控闹钟
			if (staSystem == E_NORMAL)//如果系统正常运行
			{
				RefreshTime(); //刷新时间
				RefreshDate(0);//刷新日期
			}
			if (staSystem == Dis_Alarm)
			{
				RefreshLunar(); //刷新阴历
			}
		}	
		if(flag1s && (staSystem == E_NORMAL))//1s时间到并且系统为正常运行
		{
			flag1s = 0;//清零标志位
			RefreshTemp(0);//刷新温度	
		}
   }
}

/* 温度刷新函数，读取当前温度并根据需要刷新液晶显示，
** ops-刷新选项：为0时只当温度变化才刷新，非0则立即刷新 */
void RefreshTemp(uint8 ops)
{
    int16 temp;
    uint8 pdata str[8];
    static int16 backup = 0;
    
    Get18B20Temp(&temp); //获取当前温度值
    Start18B20();        //启动下一次转换
    temp >>= 4;          //舍弃4bit小数位
    if ((backup!=temp) || (ops!=0)) //按需要刷新液晶显示
    {
        str[0] = (temp / 10) + '0';  //十位转为ASCII码
        str[1] = (temp % 10) + '0';  //个位转为ASCII码
        str[2] = '\'';             //用'C代替℃
        str[3] = 'C';
        str[4] = '\0';             //字符串结束符
        LCD1602_Dis_Str(12, 1, str);    //显示到液晶上
        backup = temp;             //刷新上次温度值
    }
}


/*初始化定时器0 定时5ms*/	
void Init_Timer0()
{
	
	TMOD = 0x01;  //定时器16为计数工作模式
	TH0 =0xee;
	TL0 =0x00; //5ms
	ET0 = 1; //开定时器0中断
	TR0 = 1;//启动定时器0
}
/*定时器0中断服务程序*/
void Timer0() interrupt 1
{
	static uint8 tmr1s = 0;
    static uint8 tmr200ms = 0;
	
	TH0 =0xee;
	TL0 =0x00; //5ms
	tmr200ms++;  //定时200ms
    if (tmr200ms >= 40)
    {
        tmr200ms = 0;
        flag200ms = 1;
        tmr1s++;  //定时1s
        if (tmr1s >= 5)
        {
            tmr1s = 0;
            flag1s = 1;
        }
    }
	KeyScan();//按键扫描
}