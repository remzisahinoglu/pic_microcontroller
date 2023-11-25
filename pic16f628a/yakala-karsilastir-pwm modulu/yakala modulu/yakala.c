// CCP biriminin yakalama özelliðinin uygulamasý -denenmedi-
// 16.04.2007
// yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

long yuksek, alcak, pals_genisligi;

#INT_CCP1
void isr()         // PIR.2 biti 0 olur = tmr1 saklayýcýsýnný deðeri yakalanmadý
{
   yuksek = CCP_1_LOW;
   alcak = CCP_1_HIGH;

   pals_genisligi = yuksek - alcak;
}

void main()
{
   while(TRUE)
   {
      setup_ccp1(CCP_CAPTURE_RE);    // CCP1 kesmesi her yükselen kenarda yakalasýn
      setup_timer_1(T1_INTERNAL);    // timer1 açýk, frekans bölücü oraný 1:1 
      
      enable_interrupts(GLOBAL);     // kesmelere izin ver
      enable_interrupts(INT_CCP1);   // CCP kesmesini kur
       
      // normal program akýþý kesme gelmesi beklenir
      // kesmede okunna pals_geniþliði deðerini ekrana yaz
   }
}
