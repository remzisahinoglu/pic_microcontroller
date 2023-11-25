//unipolar step motor ileri-geri (tam adým) kontrol uygulamasý
//05.10.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTB = 0x06
#byte i = 0x28
#byte tam_adim = 0x29

void main()
{
   delay_ms(500);
   tam_adim=i=0; 
   
   while(TRUE)
   {  
      //////// YARIM ADIM KONTROL ////////
      if(tam_adim == 0)
      {
         if(bit_test(PORTB,4)==0)
         {
            delay_ms(50);
            i=i+1;
            if(i==9){i=1;}
            switch (i)
            {                                   // yarým adým kontrol
               case 1:   output_a(8);   break;   
               case 2:   output_a(12);  break;   
               case 3:   output_a(4);   break;   
               case 4:   output_a(6);   break;
               case 5:   output_a(2);   break;   
               case 6:   output_a(3);   break;   
               case 7:   output_a(1);   break;   
               case 8:   output_a(9);   break;
            }
         }
         
         if(bit_test(PORTB,5)==0)
         {
            delay_ms(50);
            i=i-1;
            if(i<=0){i=8;}
            switch (i)
            {                                   // yarým adým kontrol 
               case 1:   output_a(8);   break;   
               case 2:   output_a(12);  break;   
               case 3:   output_a(4);   break;   
               case 4:   output_a(6);   break;
               case 5:   output_a(2);   break;   
               case 6:   output_a(3);   break;   
               case 7:   output_a(1);   break;   
               case 8:   output_a(9);   break;   
            }
         }
      }
   
      //////// TAM ADIM KONTROL ////////
      if(tam_adim == 1)
      {
         if(bit_test(PORTB,4)==0)
         {
            delay_ms(50);
            i=i+1;
            if(i==5){i=1;}
            switch (i)
            {                                   // tam adým kontrol
               case 1:   output_a(9);   break;   
               case 2:   output_a(12);  break;   
               case 3:   output_a(6);   break;   
               case 4:   output_a(3);   break;   
            }
         }
         
         if(bit_test(PORTB,5)==0)
         {
            delay_ms(50);
            i=i-1;
            if(i<=0){i=4;}
            switch (i)
            {                                   // tam adým kontrol 
               case 1:   output_a(9);   break;   
               case 2:   output_a(12);  break;  
               case 3:   output_a(6);   break;   
               case 4:   output_a(3);   break;   
            }
         }
      }
   }
}
