/*
****************串口显示DS18B20温度值************************
DS18B20数据接口为P2.2
打开串口后比特率设置为9600
接收模式为文本模式
*/
#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ
/*====================================
 自定义类型名
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

/*====================================
 硬件接口位声明
====================================*/
sbit DS  = P2^2;   //DS18B20单总线


/*====================================
函数：void Delay_Ms(INT16U ms)
参数：ms，毫秒延时形参
描述：12T 51单片机自适应主时钟毫秒级延时函数
====================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}
/*us延时函数，执行一次US--所需6.5us进入一次函数需要11.95us*/
void Delay_us(uchar us)
{
	while(us--);	
}

/*单总线初始化时序*/
bit ds_init()
{
	bit i;
	DS = 1;
	_nop_();
	DS = 0;
	Delay_us(75); //拉低总线499.45us 挂接在总线上的18B20将会全部被复位
	DS = 1; //释放总线
	Delay_us(4); //延时37.95us 等待18B20发回存在信号
	i = DS;
	Delay_us(20); //141.95us
	DS = 1;
	_nop_();
	return (i);
}
/*写一个字节*/
void write_byte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//产生些时序
		DS = dat & 0x01;
		Delay_us(10);//76.95us
		DS = 1; //释放总线准备下一次数据写入
		_nop_();
		dat >>= 1;
	}
}

uchar read_byte()
{
	uchar i, j, dat;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//产生读时序
		DS = 1;
		_nop_();//释放总线
		j = DS;
		Delay_us(10);//76.95us
		DS = 1;
		_nop_();
		dat = (j<<7)|(dat>>1);	
	}
	return (dat);
}

/*串口初始化，比特率9600*/
void UART_Init()
{
	TMOD |= 0x20;  	//T1工作模式2  8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd; 	//比特率9600
	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
}

union _Temp{  //联合体类型定义
	uchar c[2];	//其中c[0]温度的高字节，c[1]存放温度的低字节
	uint x;	//x就正好是DS18B20的温度值
};

void main()
{
	union _Temp temp;//联合体变量定义
	float i;
	UART_Init(); //串口初始化
	TI = 1;//使用printf()puts()前都应软件置位TI = 1；

	ds_init();//初始化DS18B20
	write_byte(0xcc);//发送跳跃ROM指令
	write_byte(0x44);//发送温度转换指令
	Delay_Ms(1000);
	while(1)
	{
		ds_init();//初始化DS18B20
		write_byte(0xcc);//发送跳跃ROM指令
		write_byte(0x44);//发送温度转换指令
		ds_init();//初始化DS18B20
		write_byte(0xcc);//发送跳跃ROM指令
		write_byte(0xbe);//读取DS18B20暂存器值
		temp.c[1] = read_byte();//温度低字节
		temp.c[0] = read_byte();//温度高字节						
		i = temp.x * 0.0625;//还原小数
		printf("温度: %4.2f", i); //保留小数点后两位输出
		printf("℃ \n");
		Delay_Ms(5000);//5秒发送一次
	}
}