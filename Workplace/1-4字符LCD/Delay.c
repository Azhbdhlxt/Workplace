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