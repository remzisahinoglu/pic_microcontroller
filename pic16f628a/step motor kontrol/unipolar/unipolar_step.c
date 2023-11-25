//unipolar step motor ileri-geri tam adým kontrol uygulamasý
//16.04.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
char i=0;

void main()
{
   while(TRUE)                   // sonsuz döngüde kal
   {
      if(bit_test(PORTA,0)==0)   // MOTOR ÝLERÝ
      {
         delay_ms(10);           // butondaki arktan etkilenmemesi için bekleme yapýlýr  
         i=i+1;                  // adým sayacý
         if(i==5){i=1;}          // adým5 oldugunda adým1 olarak algýla
      }
      if(bit_test(PORTA,1)==0)   // MOTOR GERÝ
      {
         delay_ms(10);           // butondaki arktan etkilenmemesi için bekleme yapýlýr
         i=i-1;                  // adým sayacý
         if(i==0){i=4;}          // adým0  oldugunda adým4 olarak algýla
      }                          // ***bekleme süresi motorun algýladýðý en düþük süre ve hýzýný belirler***       
      
      switch (i)                 // motoru yönlendir
      {                          // buradaki aþamalarýn deðerleri excel dosyasýnda açýklanmýþtýr 
         case 1: output_b(1);   break;   // aþama 1
         case 2: output_b(2);   break;   // aþama 2
         case 3: output_b(4);   break;   // aþama 3
         case 4: output_b(8);   break;   // aþama 4
      }
      // motoru normaldeyken söndür.
   }
}
