//Zamanlayýcý-0 (TMR0) dahili kaynak ile uygulamasý
//19.02.2007
//yazan = Remzi ÞAHÝNOÐLU
//
//TMR0 zamanlayýcýsýný kullanarak 1 sn lik bekleme yapmak
//
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte OPTION = 0x81
#byte INTCON = 0x0B

char i=0, a=0;

#INT_TIMER0
void timer0()
{      
   bit_clear(INTCON,2);      // TMR0 kesmesini tekrar kullanýlmasý için sýfýrlandý
   set_timer0(217);          // TMRO saklayýcýsýna 217 degeri yüklendi
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
   set_timer0(217);                                 // TMR0 registerine parantez içindeki degeri yükler
   setup_counters( RTCC_INTERNAL, RTCC_DIV_256 );   // ön bölücü oraný 1:256 ve harici kaynak kullanýmý açýk
   enable_interrupts(INT_TIMER0);                   // tmr0 kesmesine izin ver
   enable_interrupts(GLOBAL);                       // kesmelere izin ver
   
   
//   set_timer0(217);           yukarýdaki tanýmlama bu þekilde de yapýlabilir
//   OPTION = 87;               
//   INTCON = 0xA0;              
   
bas:   
   switch(a)
   {   
   case 0: output_b(0); break;
   case 1: output_b(1); break;    
   }
   goto bas;
}


