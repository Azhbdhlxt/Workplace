#include <REGX51.H>
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"
#include "IndependentKey.h"

unsigned char KeyNum;
void main()
{
    LCD_Init();
	LCD_ShowString(1,1,"Hello world!");
	while(1)
	{
		//KeyNum=MatrixKey();
		KeyNum=IndependentKey();
		if (KeyNum){
        
        LCD_ShowNum(8,1,KeyNum,2);
        
        
        
        
        
        
        
        }
	}   

}
