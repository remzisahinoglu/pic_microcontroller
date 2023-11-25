//Zamanlayýcý-0 (TMR0) dahili kaynak ile uygulamasý
//20.02.2007
//yazan = Remzi ÞAHÝNOÐLU
//
//TMR2 zamanlayýcýsýný kullanarak 1 sn lik bekleme yapmak
//
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte OPTION = 0x81
#byte INTCON = 0x0B
#byte PIR1 = 0x0C
#byte PIE = 0x8C
#byte T2CON = 0x12

char i=0, a=0;

#INT_TIMER2
void timer2()
{      
   bit_set(PIR1,1);          // TMR2 kesmesini tekrar kullanýlmasý için sýfýrlandý   
                              
   i=i+1;                  
   if(i==100)                // bu döngüde 10ms 100 kere sayýlýp 1sn elde edilir
   {      
      i=0;      
      if(a==0)               // burada led 1 ise 0 yapýlýr, 0 ise 1 yapýlýr
      {a=1;}
      else
      {a=0;}               
   }
}

void main()
{   
   while(TRUE)
   {
      set_timer2(39);                       // 10ms saniyelik gecikme yapýmý
      T2CON = 0b01111111;                   // son frekans bölücü oraný = 1/16
      bit_set(PIE,1);                       // tmr2 kesmesine izin ver
      INTCON = 0xC0;                        // kesmelere izin ver
         
   bas:   
      switch(a)
      {   
         case 0: output_b(0); break;
         case 1: output_b(1); break;    
      }
      goto bas;
   }
}
