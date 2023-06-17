/*************8*8LED点阵以一行循环滚动显示
              LED点阵旁的J24黄色跳线帽短接到GND一端
*************/
#include <REGX51.H>






typedef unsigned char u8;
typedef unsigned int  u16;

sbit SRCLK=P3^6;//移位寄存器时钟输入
sbit rCLK=P3^5; //存储寄出去时钟输入
sbit SER=P3^4;  //串行数据输入

#define LEDDZ_COL_PORT P0//点阵控制端口

u8 ghc595_buf[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};//0000 0001|0000 0010|0000 0100|0000 1000|0001 0000|0010 0000|0100 0000|1000 0000
void delay_10us (u16 ten_us)
{
	while (ten_us--);
    
}
/*******************************************************************************
* 函 数 名       : delay_ms
* 函数功能		 : ms延时函数，ms=1时，大约延时1ms
* 输    入       : ten_us
* 输    出    	 : 无
*******************************************************************************/
//void delay_ms(u16 ms)
//{
//	u16 i,j;
//	for(i=ms;i>0;i--)
//		for(j=110;j>0;j--);
//}
/*595发送函数*/
/*向74HC595写入一个字节的数据*/

void hc595_write_data (u8 dat)
{
	u8 i=0;
	for(i=0;i<8;i++)			//循环8次即可将一个字节写入寄存器中
    {
		
		SER=dat>>7;				//优先传输一个字节中的高位
		dat<<=1;   				//左移1位，次高位移到最高位
		SRCLK=0;
		delay_10us(1);			//时序微秒级，更好兼容芯片
		SRCLK=1;
		delay_10us(1);//移位寄存器时钟上升沿将端口数据送入寄存器中
			    
	
	}
	rCLK=0;
	delay_10us(1);
	rCLK=1;//存储寄存器时钟上升沿将前面写入到寄存器的数据输出
}


void main()
{
	u8 i=0;
	LEDDZ_COL_PORT=0x00;//一开始置低电平  将LED点阵列全部设置为0，即LED阴极为低电平
     while (1)
       {
			for(i=0;i<8;i++)
				{
					hc595_write_data(0x00);//消影 消除前面寄存器缓存数据
					hc595_write_data (ghc595_buf[i]);//消除前面寄存器缓存数据
					delay_10us (5000);
				}
       }







//}
///**************************************************************************************
//深圳市普中科技有限公司（PRECHIN 普中）
//技术支持：www.prechin.net
//PRECHIN
// 普中

//实验名称：矩阵按键实验
//接线说明：	
//实验现象：下载程序后，按下“矩阵按键”模块中S1-S16键，对应数码管最左边显示0-F
//注意事项：																				  
//***************************************************************************************/
//#include "reg52.h"

//typedef unsigned int u16;	//对系统默认数据类型进行重定义
//typedef unsigned char u8;

//#define KEY_MATRIX_PORT	P1	//使用宏定义矩阵按键控制口		

//#define SMG_A_DP_PORT	P0	//使用宏定义数码管段码口

////共阴极数码管显示0~F的段码数据
//u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
//				0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};	

///*******************************************************************************
//* 函 数 名       : delay_10us
//* 函数功能		 : 延时函数，ten_us=1时，大约延时10us
//* 输    入       : ten_us
//* 输    出    	 : 无
//*******************************************************************************/
//void delay_10us(u16 ten_us)
//{
//	while(ten_us--);	
//}

///*******************************************************************************
//* 函 数 名       : key_matrix_ranks_scan
//* 函数功能		 : 使用行列式扫描方法，检测矩阵按键是否按下，按下则返回对应键值
//* 输    入       : 无
//* 输    出    	 : key_value：1-16，对应S1-S16键，
//				   0：按键未按下
//*******************************************************************************/
//u8 key_matrix_ranks_scan(void)
//{
//	u8 key_value=0;

//	KEY_MATRIX_PORT=0xf7;//给第一列赋值0，其余全为1
//	if(KEY_MATRIX_PORT!=0xf7)//判断第一列按键是否按下
//	{
//		delay_10us(1000);//消抖
//		switch(KEY_MATRIX_PORT)//保存第一列按键按下后的键值	
//		{
//			case 0x77: key_value=1;break;
//			case 0xb7: key_value=5;break;
//			case 0xd7: key_value=9;break;
//			case 0xe7: key_value=13;break;
//		}
//	}
//	while(KEY_MATRIX_PORT!=0xf7);//等待按键松开	
//	
//	KEY_MATRIX_PORT=0xfb;//给第二列赋值0，其余全为1
//	if(KEY_MATRIX_PORT!=0xfb)//判断第二列按键是否按下
//	{
//		delay_10us(1000);//消抖
//		switch(KEY_MATRIX_PORT)//保存第二列按键按下后的键值	
//		{
//			case 0x7b: key_value=2;break;
//			case 0xbb: key_value=6;break;
//			case 0xdb: key_value=10;break;
//			case 0xeb: key_value=14;break;
//		}
//	}
//	while(KEY_MATRIX_PORT!=0xfb);//等待按键松开	
//	
//	KEY_MATRIX_PORT=0xfd;//给第三列赋值0，其余全为1
//	if(KEY_MATRIX_PORT!=0xfd)//判断第三列按键是否按下
//	{
//		delay_10us(1000);//消抖
//		switch(KEY_MATRIX_PORT)//保存第三列按键按下后的键值	
//		{
//			case 0x7d: key_value=3;break;
//			case 0xbd: key_value=7;break;
//			case 0xdd: key_value=11;break;
//			case 0xed: key_value=15;break;
//		}
//	}
//	while(KEY_MATRIX_PORT!=0xfd);//等待按键松开	
//	
//	KEY_MATRIX_PORT=0xfe;//给第四列赋值0，其余全为1
//	if(KEY_MATRIX_PORT!=0xfe)//判断第四列按键是否按下
//	{
//		delay_10us(1000);//消抖
//		switch(KEY_MATRIX_PORT)//保存第四列按键按下后的键值	
//		{
//			case 0x7e: key_value=4;break;
//			case 0xbe: key_value=8;break;
//			case 0xde: key_value=12;break;
//			case 0xee: key_value=16;break;
//		}
//	}
//	while(KEY_MATRIX_PORT!=0xfe);//等待按键松开
//	
//	return key_value;		
//}

///*******************************************************************************
//* 函 数 名       : key_matrix_flip_scan
//* 函数功能		 : 使用线翻转扫描方法，检测矩阵按键是否按下，按下则返回对应键值
//* 输    入       : 无
//* 输    出    	 : key_value：1-16，对应S1-S16键，
//				   0：按键未按下
//*******************************************************************************/
//u8 key_matrix_flip_scan(void)
//{
//	static u8 key_value=0;

//	KEY_MATRIX_PORT=0x0f;//给所有行赋值0，列全为1
//	if(KEY_MATRIX_PORT!=0x0f)//判断按键是否按下
//	{
//		delay_10us(1000);//消抖
//		if(KEY_MATRIX_PORT!=0x0f)
//		{
//			//测试列
//			KEY_MATRIX_PORT=0x0f;
//			switch(KEY_MATRIX_PORT)//保存行为0，按键按下后的列值	
//			{
//				case 0x07: key_value=1;break;
//				case 0x0b: key_value=2;break;
//				case 0x0d: key_value=3;break;
//				case 0x0e: key_value=4;break;
//			}
//			//测试行
//			KEY_MATRIX_PORT=0xf0;
//			switch(KEY_MATRIX_PORT)//保存列为0，按键按下后的键值	
//			{
//				case 0x70: key_value=key_value;break;
//				case 0xb0: key_value=key_value+4;break;
//				case 0xd0: key_value=key_value+8;break;
//				case 0xe0: key_value=key_value+12;break;
//			}
//			while(KEY_MATRIX_PORT!=0xf0);//等待按键松开	
//		}
//	}
//	else
//		key_value=0;		
//	
//	return key_value;		
//}

///*******************************************************************************
//* 函 数 名       : main
//* 函数功能		 : 主函数
//* 输    入       : 无
//* 输    出    	 : 无
//*******************************************************************************/
//void main()
//{	
//	u8 key=0;

//	while(1)
//	{
//	   	key=key_matrix_ranks_scan();
//		if(key!=0)
//			SMG_A_DP_PORT=gsmg_code[key-1];//得到的按键值减1换算成数组下标对应0-F段码		
//	}		
//}
}
