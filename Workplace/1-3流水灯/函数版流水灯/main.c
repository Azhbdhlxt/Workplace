#include <REGX52.H>
#include <INTRINS.H>
void Delayxms(unsigned int xms)
{
	unsigned char i,j;
	while(xms){
	_nop();
	i=4;
	j=205;
	
	do{
		
			while(--j);
		}while(--i);
	xms--;
}
}	

	void main()
		{
	while(1){
		/*P2=0xF0;
		Delayxms(600);
		P2=0x01;
		Delayxms(500);
		P2=0x02;
		Delayxms(400);*/
		P2=0xFF;//1111 1111
		P2=0xFE;//1111 1110
		Delayxms(500);
		P2=0xFD;//1111 1101
		Delayxms(500);
		P2=0xFC;//1111 1100
		Delayxms(500);
		P2=0xFB;//1111 1011
		Delayxms(500);
		P2=0xFA;//1111 1010
		Delayxms(600);
		P2=0xF9;//1111 1001
		Delayxms(300);
		P2=0xF8;
		Delayxms(400);
		P2=0xF7;//1111 0111//顺序二进制进位闪烁
		Delayxms(400);//第1 ，2 ，3， 4灯轮流亮
		//P2=0xF8;
		//Delayxms();//1,2,3和123
	}
	}

			