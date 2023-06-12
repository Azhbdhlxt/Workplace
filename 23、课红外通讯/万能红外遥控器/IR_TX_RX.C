/*FOSC=11.0592MHz*/
#include "qxmcs51_config.h"
#include "delay.h"
#include "IR_TX_RX.H"

BOOL	IR_RX_Finish;
INT8U 	IR_RX_CODE[4];
extern INT8U IR_TX_CODE[4];
void IR_TX_RX_Init()
{
	TMOD = 0x12;// 0001(timer1) 0010(timer0) 
	TH0	= 0xf4;
	TL0	= 0xf4;//38KHz 
	EA	= 1;// Enable All Interrupt
	ET1	= 1;// Enable Timer1
	ET0	= 1;// Enable Timer0
	EX0	= 1;// Enable External Interrupt0
	P_IR_TX = 1;	// stop emit
}

void Timer1_Delay(INT8U th1,INT8U tl1,BOOL mod)
{
	TH1 = th1;
	TL1 = tl1;
	TR1 = 1;//timer1 run
	TR0 = mod;//timer0 run
	while(!TF1);//wait timer1 flag
	TF1 = 0;
	TR0 = 0;//timer0 stop run
	TR1 = 0;//timer1 stop run
	P_IR_TX = 1;// stop emit
}

void Transmit_TR_CODE()
{
	INT8U num, num1, IR_CodeBuf[4];
	EX0 = 0;
	Timer1_Delay(0xdf, 0x9a, 1);//emit 9ms
	Timer1_Delay(0xef, 0xcd, 0);//delay 4.5ms
	for(num = 0; num < 4; num++)
	{
		IR_CodeBuf[num] = IR_TX_CODE[num];
		for(num1 = 0; num1 < 8; num1++)
		{
			Timer1_Delay(0xfd, 0xfc, 1);//emit 0.56ms
			if (IR_CodeBuf[num] & 0x01)
			{
				Timer1_Delay(0xf9, 0xef, 0);//delay 1.685ms
			}
			else
			{
				Timer1_Delay(0xfd, 0xfc, 0);//delay 0.56ms
			}
			IR_CodeBuf[num] >>= 1;
		}
	}
	Timer1_Delay(0xfd, 0xd7, 1);//emit 600us
	EX0 = 1;//open interrupt
}

void Timer0() interrupt 1 //1 mean timer0
{
	P_IR_TX = ~P_IR_TX;
}
void Timer1() interrupt 3
{
	
}
void External() interrupt 0//0 mean external interrupt0
{
	BOOL Start_Flag;
	INT8U num,num1;	

	EX0 = 0;//close interrupt
	TH1 = 0xe0;
	TL1 = 0x52;	//8800us
	TF1 = 0;
	TR1 = 1;// timer1 run
	Start_Flag = 1;
	while(!TF1)//timer flag	 
	{
		if(P_IR_RX)
			Start_Flag = 0;
	}
	TR1 = 0;//important
	TF1 = 0;//clear timer1 flag
	TH1 = 0xee;
	TL1 = 0x12; //4980us
	TR1 = 1;//timer1 run
	while(!TF1);
	{
		if(P_IR_RX)
			Start_Flag = 0;
	}
	TR1 = 0;//rimer1 stop run
	TF1 = 0;//clear timer1 flag
	if ((P_IR_RX == 0) && Start_Flag)
	{
		for(num = 0; num < 4 ; num++)
		{
			for(num1 = 0; num1 < 8; num1++)
			{
				IR_RX_CODE[num] >>= 1;
				while(!P_IR_RX);//wait for high level
				TH1 = 0xfc;
				TL1 = 0xFA;//840us
				TR1 = 1;//timer1 run
				while(!TF1);//wait for timer1 flag
				TR1 = 0;//timer1 run 
				TF1 = 0;//clear timer1 flag
				if(P_IR_RX)
				{
					IR_RX_CODE[num] |= 0x80;
					TH1 = 0xfb;
					TL1 = 0xf3;//1125us
					TR1 = 1;//timer1 run
					while(!TF1);//wait for timer1 flag
					TR1 = 0;//timer1 run 
					TF1 = 0;//clear timer1 flag
				}
			}
		}
		IR_RX_Finish = 1;
		for(num1 = 0; num1 < 5; num1++)
		{
			BEEP = !BEEP;
			Delay_Ms(20);
		}
		BEEP = 1;
	}
	EX0 = 1;
}