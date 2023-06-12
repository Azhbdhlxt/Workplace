/*NEC协议红外通信
单片机解码后通过串口以9600的比特率发送出去
*/
#include <reg52.h>
/*====================================
 自定义类型名
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

uchar IRtime;	  //储存检测红外高低电平持续时间
uchar IRcord[4];  //储存解码后的4个字节数据
uchar IRdata[33]; //包含起始码在内的33位数据
bit IRpro_ok;	  //解码后4个字节数据接收完成标志位
bit IRok;		  //33位数据接收完成标志

//初始化定时器0，外部中断0，串口
void init()
{
	TMOD |= 0x02;//设置定时器0工作模式2,8位自动重装
	TL0 = TH0 = 0;//初始化定时器0寄存器，定时器0溢出一次时间为256个机器周期
	EA = 1;//开总中断
	ET0 = 1;//开定时器0中断
	TR0 = 1;//启动定时器0

	IT0 = 1;//设置外部中断0跳变沿触发方式
	EX0 = 1;//开外部中断0中断

	TMOD |= 0x20;//设置定时器1工作模式2 ,8位自动重装
	TL1 = TH1 = 0xfd;//比特率9600
	SM1 =  1;//设置串口工作模式1,10位异步收发
	TR1= 1;	 //启动定时器1
}

//定时器0中断，每中断一次需要256*1.085us =  277.76us
void time0() interrupt 1
{
	IRtime++;//277.76us
}

//外部中断0 存入33次脉宽
void int0() interrupt 0
{
	static uchar i;//静态变量用于存入33次数据计数
	static bit startflag;//开始储存脉宽标志位
	if(startflag)
	{
		/*判断引导码，如果是引导码则从起始码开始存*/
		if((IRtime < 53) && (IRtime >= 32))	i = 0;
		IRdata[i] = IRtime;//以TO溢出的次数来计算脉宽把这个时间存放在数组中
		IRtime = 0;//计数清零
		i++;//计数脉宽存入次数自加
		if(i == 33)	   //i等于33那么就表示已经存入了33次脉宽
		{
			IRok = 1; //脉宽检查完成
			i = 0;	  //把脉宽计数清零准备下次存入
		}
	}
	else
	{
		IRtime = 0;	  //定时器0计数清零
		startflag = 1;//开始处理标志位置1
	}

}

//把提取的33次脉宽进行解码 NEC协议
void IRcordpro()
{
	uchar i;//i是用于计数处理4个字节
	uchar j;//j用于计数处理1个字节的8位数据
	uchar k;//k用于计数处理33次脉宽
	k = 1;//从第一位脉宽开始处理，丢掉起始码
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 8; j++)
		{
			//如果脉宽大于数据0标准的1125us那么就判定为数据1
			if(IRdata[k] > 5) IRcord[i] |= 0x80;//写1
			//只能右移7次，如果右移8次则会把第一位数据移出去
			if(j < 7) IRcord[i] >>= 1;
			k++; //处理下一次脉宽
		}
	}
	IRpro_ok = 1;//解码完成
}

void main()
{
	uchar i; //计数串口发送字节数
	init();//初始化
	while(1)
	{
		if(IRok)//判断33次脉宽是否提取完成
		{
		 	IRcordpro();//根据脉宽解码出4个字节的数据
			IRok = 0;//清零脉宽检查完成标志位等待下一次脉宽检查
			if(IRpro_ok)//判断解码是否完成
			{
				for(i = 0; i < 4; i++) //串口发送4个字节数据
				{
					SBUF = IRcord[i]; //发送数据
					while(!TI);		  //等待发送完成标志
					TI = 0;			  //清零发送完成标志位
				}
				IRpro_ok = 0;//清零解码标志位	
			}
		}
	}

}