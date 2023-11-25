//Zamanlayýcý-1 (TMR0) dahili kaynak ile uygulamasý
//20.02.2007
//yazan = Remzi ÞAHÝNOÐLU
//
//TMR1 zamanlayýcýsýný kullanarak 1 sn lik bekleme yapmak
//
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte OPTION = 0x81
#byte INTCON = 0x0B
#byte PIR1 = 0x0C

char i=0, a=0;

#INT_TIMER1
void timer1()
{      
   bit_clear(PIR1,0);        // TMR1 kesmesini tekrar kullanýlmasý için sýfýrlandý
   set_timer1(55536);        // TMRO saklayýcýsýna 217 degeri yüklendi
                             // bu deger ön bölücü degeri ayarlandýgýnda 10ms denk gelir 
   i=i+1;                  
   if(i==100)                // bu döngüde 10ms 100 kere sayýlýp 1sn elde edilir.
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
   set_timer1(55536);                               // 10ms saniyelik gecikme yapýmý
   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );     // dahili osilatör ve ön bölücü degeri 1:1
   enable_interrupts(INT_TIMER1);                   // tmr0 kesmesine izin ver
   enable_interrupts(GLOBAL);                       // kesmelere izin ver
   
bas:   
   switch(a)
   {   
   case 0: output_b(0); break;
   case 1: output_b(1); break;    
   }
   goto bas;
}
