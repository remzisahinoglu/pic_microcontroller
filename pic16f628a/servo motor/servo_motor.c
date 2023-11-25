/*
Project : servo kontrol
Version : 1.0
Date    : 21.07.2010
Author  : Remzi �AH�NO�LU
Comment : servo motor kullan�m�n� ogrenmek

periyod 20ms, duty 600us ile 2000us aras�nda de�i�ebilir.

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


long periyod=0, doluluk=0, bosluk=0;     // periyod degeri 255 den b�y�k olduug i�in log olarak tan�mland�

void main()
{

   set_tris_a (0x03);
   set_tris_b (0x00);
   output_b(0);

   periyod = 20000;                      // pwm periyodu = 20ms   olarak ayarlad�k
   doluluk = 1500;                       // doluluk oran�n� ilk olarak motoru orta konuma getirmek i�in 1.5ms olarak ayarland�
   bosluk = periyod - doluluk;           // bo�lu�u hesapla ilk enerji verildi�inde sorun ��kmas�n
   
   while(TRUE)
   {
      if(bit_test(PORTA,0)==0)                      // ARTTIR - PORTA nin 0. biti -
      {
         delay_ms(50);                              // ark s�nd�r
         doluluk = doluluk + 5;                     // de�er kadar art
         if(doluluk > 2000){doluluk = 2000;}        // �st s�n�rda kal
         bosluk = periyod - doluluk;                // bosluk s�resini ayarla
      }   

      if(bit_test(PORTA,1)==0)                      // AZALT - PORTA nin 1. biti -
      {
         delay_ms(50);
         doluluk = doluluk - 5;                     // de�er kadar azal
         if(doluluk < 600){doluluk = 600;}          // alt s�n�rda kal
         bosluk = periyod - doluluk;                // bosluk s�resini ayarla 
      }   
         
      // periyod
      output_high(PIN_B3);                          // PWM ��k�� PORTB nin 4. bitinden (RB3)
      delay_us(doluluk);                                           
      output_low(PIN_B3);
      delay_us(bosluk);                                                
                                                            
   }
}
