//dizi kavramý uygulamasý
//eepromdaki veriyi dizine yüklemek ve kullnamak
//11.03.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
#byte PORTB = 0x06


char harf[4]={};

void main()
{
   while(TRUE)
   {
      if(bit_test(PORTA,0)==0)
      {                  
         harf[0]=2;
         write_eeprom(0x01, harf[0]);
         delay_ms(100);
               
      }
      if(bit_test(PORTA,1)==0)   
      {
         harf[0]=read_eeprom(0x01);
         output_b(harf[0]);         
      }
   }
}

