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
   if(C1OUT)                        // C1 cýksýný kontrolet   
   {
      output_high(PIN_B1);          // RB1 yi yak  
   }
   if(C2OUT)                        // C2 cýksýný kontrolet
   {
      output_high(PIN_B2);          // RB2 yi yak
   }
}

void main()
{
   setup_comparator(A0_A3_A1_A2);   // 100 nolu karsýlastýrýcý aktif edildi
   enable_interrupts(INT_COMP);     // karsýlastýrýcý kesmesi aktif 
   enable_interrupts(GLOBAL);       // cevresel kesmeleraktif  
   
   while(TRUE)                        
   {
      output_low(PIN_B1);           // RB1 yi söndür
      output_low(PIN_B2);           // RB2 yi söndür 
      delay_ms(100);                  
   }
}

