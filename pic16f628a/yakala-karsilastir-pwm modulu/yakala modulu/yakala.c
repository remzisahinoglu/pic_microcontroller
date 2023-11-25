// CCP biriminin yakalama �zelli�inin uygulamas� -denenmedi-
// 16.04.2007
// yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

long yuksek, alcak, pals_genisligi;

#INT_CCP1
void isr()         // PIR.2 biti 0 olur = tmr1 saklay�c�s�nn� de�eri yakalanmad�
{
   yuksek = CCP_1_LOW;
   alcak = CCP_1_HIGH;

   pals_genisligi = yuksek - alcak;
}

void main()
{
   while(TRUE)
   {
      setup_ccp1(CCP_CAPTURE_RE);    // CCP1 kesmesi her y�kselen kenarda yakalas�n
      setup_timer_1(T1_INTERNAL);    // timer1 a��k, frekans b�l�c� oran� 1:1 
      
      enable_interrupts(GLOBAL);     // kesmelere izin ver
      enable_interrupts(INT_CCP1);   // CCP kesmesini kur
       
      // normal program ak��� kesme gelmesi beklenir
      // kesmede okunna pals_geni�li�i de�erini ekrana yaz
   }
}
