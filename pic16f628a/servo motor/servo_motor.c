/*
Project : servo kontrol
Version : 1.0
Date    : 21.07.2010
Author  : Remzi ÞAHÝNOÐLU
Comment : servo motor kullanýmýný ogrenmek

periyod 20ms, duty 600us ile 2000us arasýnda deðiþebilir.

duty    konum (derece)
----    -----
600us   -90
1500us  0
2000us  +90

*/

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTB = 0x06
#byte PORTA = 0x05


long periyod=0, doluluk=0, bosluk=0;     // periyod degeri 255 den büyük olduug için log olarak tanýmlandý

void main()
{

   set_tris_a (0x03);
   set_tris_b (0x00);
   output_b(0);

   periyod = 20000;                      // pwm periyodu = 20ms   olarak ayarladýk
   doluluk = 1500;                       // doluluk oranýný ilk olarak motoru orta konuma getirmek için 1.5ms olarak ayarlandý
   bosluk = periyod - doluluk;           // boþluðu hesapla ilk enerji verildiðinde sorun çýkmasýn
   
   while(TRUE)
   {
      if(bit_test(PORTA,0)==0)                      // ARTTIR - PORTA nin 0. biti -
      {
         delay_ms(50);                              // ark söndür
         doluluk = doluluk + 5;                     // deðer kadar art
         if(doluluk > 2000){doluluk = 2000;}        // üst sýnýrda kal
         bosluk = periyod - doluluk;                // bosluk süresini ayarla
      }   

      if(bit_test(PORTA,1)==0)                      // AZALT - PORTA nin 1. biti -
      {
         delay_ms(50);
         doluluk = doluluk - 5;                     // deðer kadar azal
         if(doluluk < 600){doluluk = 600;}          // alt sýnýrda kal
         bosluk = periyod - doluluk;                // bosluk süresini ayarla 
      }   
         
      // periyod
      output_high(PIN_B3);                          // PWM çýkýþ PORTB nin 4. bitinden (RB3)
      delay_us(doluluk);                                           
      output_low(PIN_B3);
      delay_us(bosluk);                                                
                                                            
   }
}
