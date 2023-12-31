//unipolar step motor ileri-geri tam ad�m kontrol uygulamas�
//27.02.2011
//yazan = Remzi �AH�NO�LU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

//#device PIC16F628A

#byte PORTA = 0x05
char i=0;

void main()
{
   delay_ms(500);
   output_b(0b00010000);
   
   while(TRUE)                   // sonsuz d�ng�de kal
   {
      if(bit_test(PORTA,0)==0)   // MOTOR �LER�
      {
         delay_ms(15);           // butondaki arktan etkilenmemesi i�in bekleme yap�l�r  
         i=i+1;                  // ad�m sayac�
         if(i==5){i=1;}          // ad�m5 oldugunda ad�m1 olarak alg�la
      }
      if(bit_test(PORTA,1)==0)   // MOTOR GER�
      {
         delay_ms(15);           // butondaki arktan etkilenmemesi i�in bekleme yap�l�r
         i=i-1;                  // ad�m sayac�
         if(i==0){i=4;}          // ad�m0  oldugunda ad�m4 olarak alg�la
      }                          // ***bekleme s�resi motorun alg�lad��� en d���k s�re ve h�z�n� belirler***       
      
      switch (i)                 // motoru y�nlendir
      {                          // buradaki a�amalar�n de�erleri excel dosyas�nda a��klanm��t�r 
         case 1: output_b(1);   break;   // a�ama 1
         case 2: output_b(2);   break;   // a�ama 2
         case 3: output_b(4);   break;   // a�ama 3
         case 4: output_b(8);   break;   // a�ama 4
      }
      // motoru normaldeyken s�nd�r.
   }
}
