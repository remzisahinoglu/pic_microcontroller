//Zamanlay�c�-0 (TMR0) harici kaynak ile uygulamas�
//19.02.2007
//yazan = Remzi �AH�NO�LU
//
// TMR0 zamanlay�c�s�n� kullnarak harici kaynaktan girilen sinyal ile say�c� yapmak
// ra4 ucundaki d��en kenarda olusan sinyal tmr0 saklayc�s�ndaki degeri 1 artt�r�r
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
   OPTION=0b00111000;          // TOCS=1   zamanlay�c�-0 i�in saat sinyal secme biti 
                               // TOSE=1   zamanlay�c�-0 i�aret kayna�� kenar se�me biti
                               // PSA=1    �n b�l�c� atama biti
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
