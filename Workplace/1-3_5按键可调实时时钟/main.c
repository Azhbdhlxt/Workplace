#include <Delay.h>
#include <reg51.h>
#include <lcd1602.h>

/***********************���Ŷ���************************/
sbit K1=P2^3;					//�л���=1����ʱ�䡢=2�������ӡ�=0��������
sbit K2=P2^4;					//��һ��
sbit K3=P2^5;					//��һ
sbit K4=P2^6;					//��һ
sbit beep=P2^7;					//������
/***********************��������************************/
unsigned char moshi=0;					//����ָʾ��ǰ�����ĸ�����״̬
unsigned int  timer=0;					//ϵͳ��ʱ����
unsigned char miao=0,fen=10,shi=6;		//��ʼʱ��
unsigned char buzzr_fen=12,buzzr_shi=6;	//����ʱ��
unsigned char two=0;					//ָʾÿ�������¶�Ӧ��ʱ��
unsigned char time[]={"06:10:10"};		//��ǰʱ����ʾ�ռ�
unsigned char buzzr[]={"06:12"};		//���ӻ���
/*********************************************************
�������ƣ�delay
�������ã���ʱ����
���������unsigned int i��ʱʱ��
����ֵ��  ��
***********************************************************/

/*********************************************************
�������ƣ�main
�������ã�������
�����������
����ֵ��  ��
***********************************************************/
void main()
{
	TMOD|=0X01;							//T0������ʽ1
	TH0=0XfC;							//���ó�ֵ1ms
	TL0=0X18;	
	ET0=1;								//�򿪶�ʱ��0�ж�����
	EA=1;								//�����ж�
	TR0=1;								//�򿪶�ʱ��
	LCD_Init();						//LCD1602��ʼ��
	LCD_ShowString(1,0,"TIME:");
	LCD_ShowString(2,0,"CLOCK:");
	LCD_ShowString(1,5,time);				//��ʾ��ʼʱ��
	LCD_ShowString(2,6,buzzr);			//��ʾ����ʱ��
	LCD_WriteCommand(0x0c);					//�رչ��
while(1)								//��ѭ��
{
	 if(!K1)							//��ť1�����л�
	 {
	 Delay(10000);						//��ʱ����
		  if(!K1)
		  {
				two=0;					//����ʱ��ָʾ��������
		  	  	if(moshi<2)				//ģʽ�л�
			  		moshi++;
				else
					moshi=0;
				switch(moshi)			//���ù��λ��
				{
				case 0:	LCD_WriteCommand(0x0c);//����ģʽ���رչ��
						break;
				case 1:	LCD_WriteCommand(0x0e);//ģʽ1���򿪹��
						LCD_SetCursor(1,6);	//���ù��λ��
						break;
				case 2:	LCD_WriteCommand(0x0e);//ģʽ2���򿪹��
						LCD_SetCursor(2,6);	//���ù��λ��
				}
		  }
		  while(!K1);				   	//�ȴ���ť�ɿ�
	 }
	 if(moshi==1)						//ģʽ1������ʱ��
	 {
			if(!K2)						//��һ��
			{
			   Delay(10000);			//��ʱ����
			   	if(!K2)
				{
					if(two<2)
						two++;
					else
						two=0;
				}
				if(two==0)				//���������Сʱλ��
					{
					LCD_WriteCommand(0x0e);
					LCD_SetCursor(1,6);
					}
				if(two==1)					//��������ڷ���λ��
					{
					LCD_WriteCommand(0x0e);
					LCD_SetCursor(1,9);
					}
				if(two==2)					//�����������λ��
					{
					LCD_WriteCommand(0x0e);
					LCD_SetCursor(1,12);
					}
				while(!K2);				//�ȴ���ť�ɿ�
			}
			if(two==0)
				{
				if(!K3) 				//Сʱ��һ
					{
					Delay(10000);		//��ʱ����
						if(!K3)
						{
							if(shi<23)	//����Сʱ���ֵΪ23
							   shi++;	//Сʱ��һ
							else
								shi=0;
						}
					time[0]=shi/10+0x30;//����
					time[1]=shi%10+0x30;
					LCD_ShowString(1,5,time);//��ʾ
					LCD_SetCursor(1,6);		//���ֹ��λ��
					while(!K3);
					}
				 if(!K4) 				//Сʱ��һ
					{
					Delay(10000);		//��ʱ����
						if(!K4)
						{
							if(shi>0)
							   shi--;
							else
								shi=23;
						}
					time[0]=shi/10+0x30;//����
					time[1]=shi%10+0x30;
					LCD_ShowString(1,5,time);//��ʾ
					LCD_SetCursor(1,6);
					while(!K4);
					}
				}
			if(two==1)
				{
				if(!K3) 				//���Ӽ�һ
					{
					Delay(10000);		//��ʱ����
						if(!K3)
						{
							if(fen<59)	//���Ʒ������ֵΪ59
							   	fen++;
							else
								fen=0;
						}
					time[3]=fen/10+0x30;//����
					time[4]=fen%10+0x30;
					LCD_ShowString(1,5,time);//��ʾ
					LCD_SetCursor(1,9);
					while(!K3);
					}
				 if(!K4) 				//���Ӽ�һ
					{
					Delay(10000);		//��ʱ����
						if(!K4)
						{
							if(fen>0)
							   fen--;
							else
								fen=59;
						}
					time[3]=fen/10+0x30;//����
					time[4]=fen%10+0x30;
					LCD_ShowString(1,5,time);//��ʾ
					LCD_SetCursor(1,9);
					while(!K4);
					}
				}
			if(two==2)
				{
				if(!K3) 				//���һ
					{
					Delay(10000);		//��ʱ����
						if(!K3)
						{
							if(miao<59)	//���Ʒ������ֵΪ59
							   	miao++;
							else
								miao=0;
						}
					time[3]=miao/10+0x30;//����
					time[4]=miao%10+0x30;
					LCD_ShowString(1,5,time);//��ʾ
					LCD_SetCursor(1,9);
					while(!K3);
					}
				 if(!K4) 				//���һ
					{
					Delay(10000);		//��ʱ����
						if(!K4)
						{
							if(miao>0)
							   miao--;
							else
								miao=59;
						}
					time[3]=miao/10+0x30;//����
					time[4]=miao%10+0x30;
					LCD_ShowString(1,5,time);//��ʾ
					LCD_SetCursor(1,9);
					while(!K4);
					}
				}
		}
		if(moshi==2)					//ģʽ2����������
		{
			if(!K2)						//��һ��
			{
			   Delay(10000);			//��ʱ����
			   	if(!K2)
				{
					two=!two;			//�л�����
				}
				if(two==0)				//���������Сʱ
					{
					LCD_WriteCommand(0x0e);	//�������
					LCD_SetCursor(2,6);
					}
				else					//��������ڷ���
					{
					LCD_WriteCommand(0x0e);	//�������
					LCD_SetCursor(2,9);
					}
				while(!K2);
			}
			if(two==0)
				{
				if(!K3) 				//Сʱ��һ
					{
					Delay(10000);		//��ʱ����
						if(!K3)
						{
							if(buzzr_shi<23)//����Сʱ���ֵΪ23
							   	buzzr_shi++;
							else
								buzzr_shi=0;
						}
					buzzr[0]=buzzr_shi/10+0x30;//����
					buzzr[1]=buzzr_shi%10+0x30;
					LCD_ShowString(2,6,buzzr);//��ʾ
					LCD_SetCursor(2,6);
					while(!K3);
					}
				 if(!K4) 				//Сʱ��һ
					{
					Delay(10000);		//��ʱ����
						if(!K4)
						{
							if(buzzr_shi>0)
							   buzzr_shi--;
							else
								buzzr_shi=23;
						}
					//buzzr[0]=buzzr_sh;
					}}
				}}}	