//do while komutu uygulamas�
//05.04.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

void main()
{
char x=0;
   
   do                     // x 5 oluncaya kadar x e +1 uygulana�r
   {                      // x 5 oldugunda ise while �n alt sat�r�ndan 
      x=x+1;              // program cal�smaya devam eder
      output_b(x);
      delay_ms(500);
   }while(x != 5);          // dogru oldugu s�rece d�n�g�y� i�le
   
   output_b(255);
}
