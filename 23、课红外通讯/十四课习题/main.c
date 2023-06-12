/*
DS18B20测温程序数码管显示
可测温显示范围为-55℃ 至 +125℃

*/
#include <reg52.h>
#include <intrins.h>
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
sbit DU  = P2^6;   //数码管段选
sbit WE  = P2^7;   //数码管位选
/*显示负号*/
uchar s=0;
/*====================================
共阴极数码管段选码
====================================*/
uchar code table[]={ 
//0		1	 2     3     4     5     6     7     8
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//9     A     B	   C	 D	   E	 F		-	 .	  关显示
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0x80, 0x00
                   };

/*====================================
数码管位选码
====================================*/
				  //第1位	2位	  3位	 4位   5位	6位	  7位	8位
uchar code T_COM[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};//数码管位码

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
/*====================================
函数：void Display(unsigned long Value)
参数：Value,显示值 32位
描述：
====================================*/
void Display(unsigned long Value)		
{
	uchar i=0;
//第一位数码管
//------------------------------
	if(s == 0);						   	//如果为负数则不显示
	else
	{		
		DU = 0;							//关闭段选
		P0 = s;		
		DU = 1;							//打开段选
		DU = 0;							//关闭段选
	
		WE = 0;						//关闭位选
		P0 = T_COM[i++];			
		WE = 1;						//打开位选
		WE = 0;						//关闭位选
		Delay_Ms(1);
	}
//第二位数码管
//------------------------------
	if(Value < 1000000);			   //如果温度值百位为0则不显示
	else
	{
		DU = 0;							//关闭段选
		P0 = table[Value/1000000];	
		DU = 1;							//打开段选
		DU = 0;							//关闭段选
	
		WE = 0;						//关闭位选
		P0 = T_COM[i++];				
		WE = 1;						//打开位选
		WE = 0;						//关闭位选
		Delay_Ms(1);
	}
//第三位数码管
//------------------------------
	if(Value < 100000);					
	else
	{
		DU = 0;							//关闭段选
		P0 = table[Value%1000000/100000];		//数码管显示百位
		DU = 1;							//打开段选
		DU = 0;							//关闭段选
	
		WE = 0;						//关闭位选
		P0 = T_COM[i++];				
		WE = 1;						//打开位选
		WE = 0;						//关闭位选
		Delay_Ms(1);
	}
//第四位数码管
//-------------------------------
	DU = 0;

	P0 = table[Value%100000/10000]|0x80;//显示温度小数点，温度值个位显示

	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[i++];		
	WE = 1;
	WE = 0;
	Delay_Ms(1);
//第五位数码管
//-------------------------------
	DU = 0;							//关闭段选
	P0 = table[Value%10000/1000];	
	DU = 1;							//打开段选
	DU = 0;							//关闭段选

	WE = 0;						//关闭位选
	P0 = T_COM[i++];				
	WE = 1;						//打开位选
	WE = 0;						//关闭位选
	Delay_Ms(1);
//第六位数码管
//-------------------------------
	if((Value%100/10) == 0 && (Value%1000/100) == 0 ); 
	else
	{
		DU = 0;							//关闭段选
		P0 = table[Value%1000/100];	
		DU = 1;							//打开段选
		DU = 0;							//关闭段选
	
		WE = 0;						//关闭位选
		P0 = T_COM[i++];				
		WE = 1;						//打开位选
		WE = 0;						//关闭位选
		Delay_Ms(1);
	}
//第七位数码管
//-------------------------------
	if((Value%10) == 0 && (Value%100/10) == 0);
	else
	{
		DU = 0;							//关闭段选
		P0 = table[Value%100/10];	
		DU = 1;							//打开段选
		DU = 0;							//关闭段选
	
		WE = 0;						//关闭位选
		P0 = T_COM[i++];				
		WE = 1;						//打开位选
		WE = 0;						//关闭位选
		Delay_Ms(1);
	}
//第八位数码管
//-------------------------------
	if((Value%10) == 0 );
	else
	{
		DU = 0;							//关闭段选
		P0 = table[Value%10];	
		DU = 1;							//打开段选
		DU = 0;							//关闭段选
	
		WE = 0;						//关闭位选
		P0 = T_COM[i++];				
		WE = 1;						//打开位选
		WE = 0;						//关闭位选
		Delay_Ms(1);
	}
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
void main()
{
	unsigned long temp;
	uint i;
	uchar L, M;
	ds_init();//初始化DS18B20
	write_byte(0xcc);//发送跳跃ROM指令
	write_byte(0x4e);//写暂存器指令
	write_byte(0x7f);//TH用户字节
	write_byte(0xf7);//TL用户字节
	write_byte(0x7f);//配置工作在12位模式下
	ds_init();//初始化DS18B20
	write_byte(0xcc);//发送跳跃ROM指令 
	write_byte(0x48);//拷贝暂存器到温度传感器的EEPROM中
	while(1)
	{
		ds_init();//初始化DS18B20
		write_byte(0xcc);//发送跳跃ROM指令
		write_byte(0x44);//发送温度转换指令
		ds_init();//初始化DS18B20
		write_byte(0xcc);//发送跳跃ROM指令
		write_byte(0xbe);//读取DS18B20暂存器值
		L = read_byte();
		M = read_byte();
		//模拟显示-55度，实际测温需注解掉下面这两句
		//M = 0xfe;
		//L = 0x6f;

		i = M;
		i <<= 8;
		i |= L;		
		if(M >= 0x08)	//判断是否为负数
		{ 
			i = ~i + 1;//负数是以补码的形式存放的需要我们需要取反加1
			s = 0x40;  //显示负数符号
		}
		else s = 0;	   //为正数则不显示				
		temp = i * 0.0625 * 10000; //浮点型数据转换
		Display(temp);
	}
}