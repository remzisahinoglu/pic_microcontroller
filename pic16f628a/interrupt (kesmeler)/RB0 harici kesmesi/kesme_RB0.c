//RB0/INT harici kesme uygulamasý
//12.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte STATUS = 0x03
#byte OPTION = 0x81
#byte INTCON = 0x0B

#INT_EXT                           // kesme geldiðinde program akýsý
void RB_isr()                      // buradan devam eder
{                     
bas:     
   output_a(1);                    // 1 degeri a portuna gönderilir.     
   bit_clear(INTCON,1);            // RB0/INT harici kesme bayraðý  
   delay_ms(100);                  // tekrar kullanýlmak için sýfýrlanýr
                                   // butona basýlmak için beklenir 
   if(input(PIN_B0)==0)            // kesme sinyali var mý yok mu?
   {                                  
      goto bas;                    // var ise bas a dön iþlemkeri tekrar uygula 
   }                               // yok ise kesmeyi bitir
}                                  // kaldýðýn yerden devam et  

void main()
{  
   set_tris_b(1);                  // RB0/INT ucu giriþ yapýlýr
   enable_interrupts(INT_EXT);     // RB0 harici kesmesi aktif yapýlýr
   enable_interrupts(GLOBAL);      // kesmelere izin verilir
   bit_clear(OPTION,6);            // kesme düþen kenarda olussun
                
tekrar:                            // pice ilk enerji verildiðinde 
   output_a(1);                    // 1-2-4-8 olarak 1 sn aralýkla      
   delay_ms(1000);                 // saymaya baslar    
   output_a(2);                    // bu deðerler portaya çýkýþ olarak verilir
   delay_ms(1000);
   output_a(4);
   delay_ms(1000);
   output_a(8);
   delay_ms(1000);
   goto tekrar;   
}

