//Zamanlayýcý-0 (TMR0) harici kaynak ile uygulamasý
//19.02.2007
//yazan = Remzi ÞAHÝNOÐLU
//
// TMR0 zamanlayýcýsýný kullnarak harici kaynaktan girilen sinyal ile sayýcý yapmak
// ra4 ucundaki düþen kenarda olusan sinyal tmr0 saklaycýsýndaki degeri 1 arttýrýr
//
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte OPTION = 0x81
#byte TMR0 = 0x01

void main()
{
   OPTION=0b00111000;          // TOCS=1   zamanlayýcý-0 için saat sinyal secme biti 
                               // TOSE=1   zamanlayýcý-0 iþaret kaynaðý kenar seçme biti
                               // PSA=1    ön bölücü atama biti
   output_b(0);
   TMR0=0;
   while(TRUE)
   {      
      output_b(TMR0);                                
      if (TMR0 == 255)    // tmr0 doldugu zaman 0 la
      {
         TMR0 = 0;
      }                 
   }                                
}
