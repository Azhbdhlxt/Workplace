/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	8*8点阵循环显示 “电”“子”		   			            			    
* 【使用说明】： 	正确安装上点阵模块
**********************************************************************************/
#include <reg52.h>
#include <intrins.h> //循环右移头文件
sbit DIO = P3^4;  //串行数据口
sbit S_CLK = P3^5;//移位寄存器时钟
sbit R_CLK = P3^6;//输出锁存器时钟
/*点阵字形码*/
unsigned char code tabel[2][8]={

0xE0,0xEE,0x01,0x6D,0x01,0x6D,0x01,0xEF,//电
0xE7,0xF7,0xF7,0xF7,0x80,0xF7,0xFB,0xC3//子
};
/*595发送一字节*/
void Send_Byte(unsigned char dat)
{
	unsigned char i; //循环次数变量
	S_CLK = 0;//拉低移位寄存器时钟
	R_CLK = 0;//拉低输出锁存器时钟
	for(i=0; i<8; i++) //循环8次
	{
		if(dat & 0x01)//发送1
			DIO = 1;
			else	  //发送0
			DIO = 0;
		dat >>= 1;//数据右移
		S_CLK = 1;//拉高移位寄存器时钟，数据移位
		S_CLK = 0;//拉低移位寄存器时钟
	}	
}
void main()
{
	unsigned char j, k, ROW;//j发送8列和8行字形码，k字符数量，ROW行值
	unsigned int z;	//动态扫描延时变量
	while(1)
	{
		for(k=0; k<2; k++)//k 需要显示的字符数量
		{
			for(z=0; z<1000; z++)//z刷新次数
			{
				ROW = 0x80;//行选初值
				for(j=0; j<8; j++) //循环8次发送行和列值
				{
					Send_Byte(tabel[k][j]);//发送列选值
					Send_Byte(ROW);	//发送行选值
					R_CLK = 1; //拉高输出锁存器，把移位寄存器中数据输出
					R_CLK = 0; //拉低输出锁存器
					ROW = _cror_(ROW, 1);//右移，选择下一行	
				}
			}
		}
	}
}