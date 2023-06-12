#include <REGX51.H>


#define SMG_A_DP_PORT  P0	//数码管定义管脚 A~DP  8段 P0口
typedef unsigned  int u16;
typedef unsigned  char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,
				  0x6d,0x7d,0x07,0x7f,0x6f,
				  0x77,0x7c,0x39,0x5e,0x79,0x71};			//g  global代表全局变量. 共阴极0-F

void delay_10us (u16 ten_us)
{
	while (ten_us--);
    
}



void smg_display ()
{
	u8 i=0;
for(i=0;i<8;i++) 						//i=7;i>=0;i--
{										//76543210
	delay_10us (1000);
	switch(i)
	{
		case 0:LSC=1;LSB=1;LSA=1;break;
		delay_10us (1000);
		case 1:LSC=1;LSB=1;LSA=0;break;
		delay_10us (1000);
		case 2:LSC=1;LSB=0;LSA=1;break;
		delay_10us (1000);
		case 3:LSC=1;LSB=0;LSA=0;break;
		delay_10us (1000);
		case 4:LSC=0;LSB=1;LSA=1;break;
		delay_10us (1000);
		case 5:LSC=0;LSB=1;LSA=0;break;
		delay_10us (1000);
		case 6:LSC=0;LSB=0;LSA=1;break;
		delay_10us (1000);
		case 7:LSC=0;LSB=0;LSA=0;break;
		delay_10us (1000);
		
	}
	SMG_A_DP_PORT=gsmg_code[7-i];
	delay_10us (1000);				//省略亮度低
	SMG_A_DP_PORT=0x00;             //消影
}	
}	


 void main()

{
	u8 i=0;
	
//  while(1)
//   {
//	   SMG_A_DP_PORT=gsmg_code[0];
//	   delay_10us (10000);
	
//	}
//---------------------------------------------------------	   
   while (1)
{
	smg_display ();
} 

}