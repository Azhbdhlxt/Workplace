/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	4*4矩阵键盘与4位独立键盘识别		   			            			    
* 【使用说明】： 	按下矩阵键盘和独立键盘任意键，数码管显示相应数值
					初始显示“-”横
**********************************************************************************/
#include <reg52.h>//包含51头文件
#include <intrins.h>//包含移位标准库函数头文件

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//数码管段选
sbit WE = P2^7;//数码管段选
uchar num;//数码管显示的值
uchar KeyValue = 20;//按键值 显示-

//共阴数码管段选表
uchar  code tabel[]= {
//0		1	 2     3     4     5     6     7     8
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//9     A     B	   C	 D	   E	 F		H	 L	 
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x76, 0x38,
//n	   u	  -	  熄灭
0x37, 0x3E, 0x40, 0x00 };

/*====================================
函数	： delay(uint z)
参数	：z 延时毫秒设定，取值范围0-65535
返回值	：无
描述	：12T/Fosc11.0592M毫秒级延时
====================================*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 
/*====================================
函数	：KeyScan()
参数	：无
返回值	：无
描述	：4*4矩阵键盘与独立键盘扫描
按键按下KeyValue全局变量值发生相应变化
====================================*/
void KeyScan()
{
	//4*4矩阵键盘扫描
	P3 = 0XF0;//列扫描
	if(P3 != 0XF0)//判断按键是否被按下
	{
		delay(10);//软件消抖10ms
		if(P3 != 0XF0)//判断按键是否被按下
		{
			switch(P3) //判断那一列被按下
			{
				case 0xe0:	KeyValue = 0;	break;//第一列被按下
				case 0xd0:	KeyValue = 1;	break;//第二列被按下
				case 0xb0:	KeyValue = 2;	break;//第三列被按下
				case 0x70:	KeyValue = 3;	break;//第四列被按下 
			}
			P3 = 0X0F;//行扫描
			switch(P3) //判断那一行被按下
			{
				case 0x0e:	KeyValue = KeyValue;	break;//第一行被按下
				case 0x0d:	KeyValue = KeyValue + 4;	break;//第二行被按下
				case 0x0b:	KeyValue = KeyValue + 8;	break;//第三行被按下
				case 0x07:	KeyValue = KeyValue + 12;	break;//第四行被按下 
			}
			while(P3 != 0X0F);//松手检测	
		}
	}
	P3 = 0XFF;//独立按键扫描
	if(P3 != 0XFF)
	{
		delay(10);//软件消抖10ms
		if(P3 != 0XFF)
		{
			switch(P3) //判断那一行被按下
			{
				case 0xfe:	KeyValue = 16;	break;//S2被按下
				case 0xfd:	KeyValue = 17;	break;//S3被按下
				case 0xfb:	KeyValue = 18;	break;//S4被按下
				case 0xf7:	KeyValue = 19;	break;//S5被按下 
			}
			while(P3 != 0XFF);//松手检测			
		}	
	}

}

void main()//main函数自身会循环
{
	WE = 1;//打开位选锁存器
	P0 = 0XFE; //1111 1110
	WE = 0;//锁存位选数据

	DU = 1;//打开段选锁存器
	while(1)
	{
		KeyScan();//20个按键键盘扫描
		P0 = tabel[KeyValue];//显示按键值
	}	
}  