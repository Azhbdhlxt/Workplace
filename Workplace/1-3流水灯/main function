#include <REGX52.H>
#include <INTRINS.H>
void Delay500ms()
{
	unsigned char i,j,k;
	_nop_();
	i=4;
	j=205;
	k=187;
	do{
		do{
			while(--k);
		}while(--j);
	}while(--i);
}
	void main()
		{
	while(1){
		/*P2=0xF0;
		Delay500ms();
		P2=0x01;
		Delay500ms();
		P2=0x02;
		Delay500ms();*/
		P2=0xFF;//1111 1111
		P2=0xFE;//1111 1110
		Delay500ms();
		P2=0xFD;//1111 1101
		Delay500ms();
		P2=0xFC;//1111 1100
		Delay500ms();
		P2=0xFB;//1111 1011
		Delay500ms();
		P2=0xFA;//1111 1010
		Delay500ms();
		P2=0xF9;//1111 1001
		Delay500ms();
		P2=0xF8;
		Delay500ms();
		P2=0xF7;//1111 0111//顺序二进制进位闪烁
		Delay500ms();//第1 ，2 ，3， 4灯轮流亮
		//P2=0xF8;
		//Delay500ms();//1,2,3和123
	}
	}

			