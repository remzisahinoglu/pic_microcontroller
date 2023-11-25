//port okuma ve test uygulamasý
//05.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

void gonder(char a)
{
output_b(a);
return;
}

void main()
{
char a;
tekrar:
set_tris_a(1);
a=input_a();
if (bit_test(a,1))         //x in 1. bitini test et. yani RA0 ý
   {   
   gonder(a);              //parentez içindeki fonk un geri dönüþ degeridir
   }
   goto tekrar;
}
