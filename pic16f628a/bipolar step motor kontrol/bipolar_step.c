//bipolar step motor ileri-geri tam adým kontrol uygulamasý
//02.06.2011
//yazan = Remzi ÞAHÝNOÐLU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,NOPUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
char i=1;

void main()
{
   while(TRUE)                   // sonsuz döngüde kal
   {
      if(bit_test(PORTA,0)==0)   // MOTOR ÝLERÝ
      {
         delay_ms(2);          // butondaki arktan etkilenmemesi için bekleme yapýlýr  
         i=i+1;                  // adým sayacý
         if(i==5){i=1;}          // adým5 oldugunda adým1 olarak algýla
      }
      if(bit_test(PORTA,1)==0)   // MOTOR GERÝ
      {
         delay_ms(2);          // butondaki arktan etkilenmemesi için bekleme yapýlýr
         i=i-1;                  // adým sayacý
         if(i==0){i=4;}          // adým0  oldugunda adým4 olarak algýla
      }
      
      switch (i)                 // motoru yönlendir
      {                          // buradaki aþamalarýn deðerleri excel dosyasýnda açýklanmýþtýr 
         case 1: output_b(6);   break;   // adým 1
         case 2: output_b(5);   break;   // adým 2
         case 3: output_b(9);   break;   // adým 3
         case 4: output_b(10);  break;   // adým 4 
      }
   }
}
