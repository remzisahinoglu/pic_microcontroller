//Kar��la�t�r�c�-1 (C1) uygulamas�
//15.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte CMCON = 0x1F

#INT_COMP                           // C2out 1 oldugunda program ak�s� buradan devam eder   
void comp_isr() 
{
   if(C1OUT)                        // C1 c�ks�n� kontrolet   
   {
      output_high(PIN_B1);          // RB1 yi yak  
   }
   if(C2OUT)                        // C2 c�ks�n� kontrolet
   {
      output_high(PIN_B2);          // RB2 yi yak
   }
}

void main()
{
   setup_comparator(A0_A3_A1_A2);   // 100 nolu kars�last�r�c� aktif edildi
   enable_interrupts(INT_COMP);     // kars�last�r�c� kesmesi aktif 
   enable_interrupts(GLOBAL);       // cevresel kesmeleraktif  
   
   while(TRUE)                        
   {
      output_low(PIN_B1);           // RB1 yi s�nd�r
      output_low(PIN_B2);           // RB2 yi s�nd�r 
      delay_ms(100);                  
   }
}

