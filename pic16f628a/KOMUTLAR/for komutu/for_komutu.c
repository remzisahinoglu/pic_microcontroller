// for komutu uygu�lams�
//08.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses PUT, MCLR, INTRC_IO, NOWDT, NOLVP, NOBROWNOUT, NOPROTECT
#endif

void main()
{
char i=0;   
   for (i=0; i<=10; i=i+1)       //ifade1 = degi�keni tan�mla
   {                             //ifade2 = ko�ul belirle
      delay_ms(200);             //ifade3 = artt�r�m  
      output_b(i);               //ifade dogru ise d�ng� i�i tekrarlan�r
   }   
                                 //ifade yanl�� ise d�ng� d�s�ndan devam edilir
}                                
