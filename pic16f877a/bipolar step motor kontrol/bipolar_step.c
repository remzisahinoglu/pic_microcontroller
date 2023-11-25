//bipolar step motor ileri-geri tam ad�m kontrol uygulamas�
//02.06.2011
//yazan = Remzi �AH�NO�LU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,NOPUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
char i=1;

void main()
{
   while(TRUE)                   // sonsuz d�ng�de kal
   {
      if(bit_test(PORTA,0)==0)   // MOTOR �LER�
      {
         delay_ms(2);          // butondaki arktan etkilenmemesi i�in bekleme yap�l�r  
         i=i+1;                  // ad�m sayac�
         if(i==5){i=1;}          // ad�m5 oldugunda ad�m1 olarak alg�la
      }
      if(bit_test(PORTA,1)==0)   // MOTOR GER�
      {
         delay_ms(2);          // butondaki arktan etkilenmemesi i�in bekleme yap�l�r
         i=i-1;                  // ad�m sayac�
         if(i==0){i=4;}          // ad�m0  oldugunda ad�m4 olarak alg�la
      }
      
      switch (i)                 // motoru y�nlendir
      {                          // buradaki a�amalar�n de�erleri excel dosyas�nda a��klanm��t�r 
         case 1: output_b(6);   break;   // ad�m 1
         case 2: output_b(5);   break;   // ad�m 2
         case 3: output_b(9);   break;   // ad�m 3
         case 4: output_b(10);  break;   // ad�m 4 
      }
   }
}
