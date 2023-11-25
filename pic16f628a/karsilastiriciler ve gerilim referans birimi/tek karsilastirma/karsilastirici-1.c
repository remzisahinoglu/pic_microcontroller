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
   output_high(PIN_B0);             // RB0 a bagl� olan ledi yak
}

void main()
{
   setup_comparator(NC_NC_A1_A2);   // 101 nolu kars�last�r�c� aktif edildi
   enable_interrupts(INT_COMP);     // kars�last�r�c� kesmesi aktif 
   enable_interrupts(GLOBAL);       // cevresel kesmeleraktif  
   
   while(TRUE)                        
   {
      output_low(PIN_B0);           // RB0 a bagl� olan ledi s�nd�r
      delay_ms(100);                  
   }
}
