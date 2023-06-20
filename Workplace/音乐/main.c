#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char
sbit voice=P2^5;
uchar code sound[]={0xff,
                    0x39,0x20,0x33,0x10,0x30,0x20,0x2b,0x30,0x26,0x40,0x24,0x60,0x20,0x80,0x00,
                   };
uchar zdjs=0, jp;
del(yj);
void main(void)
  {
      uint dpjs=0;
      uchar yj;
      TMOD=0x01,IE=0x82;
      TH0=0xd8, TL0=0xef;
   TR0=1;
while(1)
   {  
      zdjs=0;
      dpjs++; yj=sound[dpjs]; 
     dpjs++; jp=sound[dpjs];
   while(zdjs!=jp)
       { 
      if(yj!=0xff)
   {
             if(yj!=0)
               {
             voice=!voice;
                 del(yj);
                }
                else
          {
          dpjs=0; 
                      break;
                     }
    }
    else
      {
        voice=0;
                 del(jp);
    }
      }
       }
   }
time0() interrupt 1  using 1
      {
         TH0=0xd8, TL0=0xef;
         zdjs++;
           }
del(yj)
    {
      uchar yj2=2;
       while(yj!=0)
         {      
               while(yj2!=0)
                  {
                     yj2--;
                     }
                  yj2=2;
             yj--;  
       }
   
     }