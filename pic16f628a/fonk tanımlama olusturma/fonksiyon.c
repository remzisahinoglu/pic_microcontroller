//port okuma ve test uygulamas�
//05.02.2007
//yazan = Remzi �AH�NO�LU
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
if (bit_test(a,1))         //x in 1. bitini test et. yani RA0 �
   {   
   gonder(a);              //parentez i�indeki fonk un geri d�n�� degeridir
   }
   goto tekrar;
}
