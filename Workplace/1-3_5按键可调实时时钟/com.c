#include <reg51.h>
 
typedef unsigned char u8;
typedef unsigned int u16;
 
sbit RW=P2^5;
sbit RS=P2^6;
sbit EN=P2^7;
u8 code table[15]="Hello World";
 
/*延时函数*/
void delay(u16 i)
{
   while(i--);
 
}
/*写指令函数*/
void LCD_WriteCommand(u8 con){
    P0=con;           //送出指令
	  RS=0;RW=0;EN=1;  //写指令时序
	  delay(200);
	  EN=0;
}
/*写数据函数*/
void write_dat(u8 dat){
   P0=dat;		     //送出数据
	 RS=1;RW=0;EN=1;//写数据时序
	 delay(200);
	 EN=0;
}
/*LCD初始化函数*/
void LcdInit(){
   write_con(0x01);//清屏
	 write_con(0x38);//设置16*2显示
	 write_con(0x0c);//开显示
	 write_con(0x06);//写入数据时光标右移1位
}
 
/*LCD显示函数*/
void LcdDisplay(u8  table[]){
    u8 i=0;
	write_con(0x80);   //起点为第一行
	while(table[i]!='\0'){
	   write_dat(table[i]);
	   delay(100);
	   i++;
	 }
	/*
	  i=0;
	write_con(0xc0);  //起点为第二行
	while(table[i]!='\0'){
	   write_dat(table[i]);
	   delay(100);
	   i++;
	 }
	 i=0;
	 */
}
 
/*主函数*/
void main()
{
   LcdInit();
	 while(1)
	 {
   LcdDisplay(table);
	 }
}