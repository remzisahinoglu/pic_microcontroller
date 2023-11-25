//unipolar step motor belirlenen kadar adým ileri geri kontrol uygulamasý
//18.03.2011
//yazan = Remzi ÞAHÝNOÐLU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
char i=0;
int16 a=0;


void main()
{  
   delay_ms(2000);
   while(TRUE) 
   {
      output_b(0b00010000);
      
      dur:
      if(bit_test(PORTA,0)==0)   // Start
      {
            
         //************  2 tur ileri 30ms         
         a=0;
         while(a<2640)     // bu kadar adým git
         {
            i=i+1;                  // adým sayacý
            if(i==5){i=1;}
            switch (i)                 
            {                          
               case 1: output_b(1);   break;   // aþama 1
               case 2: output_b(2);   break;   // aþama 2
               case 3: output_b(4);   break;   // aþama 3
               case 4: output_b(8);   break;   // aþama 4
            }
            delay_ms(15);
            a=a+1;
            if(bit_test(PORTA,1)==0){goto dur; output_b(0);}
         }
         delay_ms(1000);
         //************  2 tur geri 30ms
         a=0;
         while(a<2640)     // bu kadar adým git
         {
            i=i-1;                  // adým sayacý
            if(i==0){i=4;}
            switch (i)                 
            {                          
               case 1: output_b(1);   break;   // aþama 1
               case 2: output_b(2);   break;   // aþama 2
               case 3: output_b(4);   break;   // aþama 3
               case 4: output_b(8);   break;   // aþama 4
            }
            delay_ms(15);
            a=a+1;
            if(bit_test(PORTA,1)==0){goto dur; output_b(0);}
         }
         delay_ms(1000);
      }
   }
}
