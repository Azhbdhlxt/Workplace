#include <REGX51.H>

typedef unsigned int u16;
typedef unsigned char u8;

#define KEY1_PRESS   1
#define KEY2_PRESS   2
#define KEY3_PRESS   3
#define KEY4_PRESS   4
#define KEY_UNPRESS  0

sbit KEY1=P3^1;
sbit KEY2=P3^0;
sbit KEY3=P3^2;
sbit KEY4=P3^3;

sbit LED1=P2^0;
sbit LED2=P2^1;
sbit LED3=P2^2;
sbit LED4=P2^3;
void delay_10us (u16 ten_us)
{
	while (ten_us--);
}
u8 key_scan(u8 mode)
{
	static u8 key=1;//进行一次
	if(mode)key=1;	//进行多次扫描
	if (key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	{
    
		delay_10us(1000);//消抖    
		key=0;		
		if(KEY1==0)
			return KEY1_PRESS;
		else if(KEY2==0)
			return KEY2_PRESS;
		else if(KEY3==0)
			return KEY3_PRESS;
		else if(KEY4==0)
			return KEY4_PRESS;

	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
    {
		key=1;
	}
    return KEY_UNPRESS;
    
    
    
    
    }

    

void main()
{
	u8 key=0;
       while (1)
       {
		  key= key_scan(0);
		   if(key==KEY1_PRESS)
			   LED1=~LED1;
		       
//		   		 else if(key==KEY2_PRESS)
//			   LED2=~LED2;
//				   		else if(key==KEY3_PRESS)
//			   LED3=~LED3;
//						   	else if(key==KEY4_PRESS)
//			   LED4=~LED4;
//	亮了不灭 ，再按才灭 
		   		   if(key==KEY2_PRESS)
			   LED2=~LED2;
				   		   if(key==KEY3_PRESS)
			   LED3=~LED3;
						   		   if(key==KEY4_PRESS)
			   LED4=~LED4;
			
       }

}