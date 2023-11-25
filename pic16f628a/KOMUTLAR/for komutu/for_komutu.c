// for komutu uyguþlamsý
//08.02.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses PUT, MCLR, INTRC_IO, NOWDT, NOLVP, NOBROWNOUT, NOPROTECT
#endif

void main()
{
char i=0;   
   for (i=0; i<=10; i=i+1)       //ifade1 = degiþkeni tanýmla
   {                             //ifade2 = koþul belirle
      delay_ms(200);             //ifade3 = arttýrým  
      output_b(i);               //ifade dogru ise döngü içi tekrarlanýr
   }   
                                 //ifade yanlýþ ise döngü dýsýndan devam edilir
}                                
