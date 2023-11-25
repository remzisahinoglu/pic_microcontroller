//do while komutu uygulamasý
//05.04.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

void main()
{
char x=0;
   
   do                     // x 5 oluncaya kadar x e +1 uygulanaýr
   {                      // x 5 oldugunda ise while ýn alt satýrýndan 
      x=x+1;              // program calýsmaya devam eder
      output_b(x);
      delay_ms(500);
   }while(x != 5);          // dogru oldugu sürece dönügüyü iþle
   
   output_b(255);
}
