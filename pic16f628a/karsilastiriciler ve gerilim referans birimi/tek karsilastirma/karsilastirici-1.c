//Karþýlaþtýrýcý-1 (C1) uygulamasý
//15.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte CMCON = 0x1F

#INT_COMP                           // C2out 1 oldugunda program akýsý buradan devam eder   
void comp_isr() 
{
   output_high(PIN_B0);             // RB0 a baglý olan ledi yak
}

void main()
{
   setup_comparator(NC_NC_A1_A2);   // 101 nolu karsýlastýrýcý aktif edildi
   enable_interrupts(INT_COMP);     // karsýlastýrýcý kesmesi aktif 
   enable_interrupts(GLOBAL);       // cevresel kesmeleraktif  
   
   while(TRUE)                        
   {
      output_low(PIN_B0);           // RB0 a baglý olan ledi söndür
      delay_ms(100);                  
   }
}
