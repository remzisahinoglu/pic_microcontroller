//usart birimi seri ileti�im verici uygulamas�
//21.02.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
#byte SPRG = 0x99
#byte TXSTA = 0x98
#byte RCSTA = 0x18
#byte TXREG = 0x19

#bit SPEN = 0x18.7

void main()
{
   set_tris_a(0b00000011);           // RA0 ve RA1 ucunda anahtar var
   set_tris_b(0b00000010);           // RB1/RX=giri�, RB2/TX=��k��
   SPRG = 25;                        // bound h�z� 9600 olara kayarland�     
   TXSTA = 0b00100100;               // BRGH=1(y�ksek h�z), TXEN=1(g�nderme aktif)
   bit_set(RCSTA,7);                 // seri ileti�im aktif (SPEN)
   
   delay_ms(100);                    // biraz bekle acele etme :)
// veri g�nderme rutinleri
bas:   
// butona bas�l� tutunca veri g�nderilir, parmak butondan �ekildi�inde g�nderilen veri tekrar s�f�r olur         
   if(bit_test(PORTA,1)==0)          // RA1 butona bas�l� m�?
   {                                 // bas�l� burdan devam et
      delay_ms(200);                 // butondan parmag� �ekme s�resi
      output_high(PIN_B2);           // hatt� 1'e �ek
      TXREG = 0x6A;                  // veriyi TXREG saklay�c�s�na y�kle
      tekrar1:
      if(bit_test(TXSTA,1)==0)       // veri g�nderimi bitti mi   
      {                              // TMRT=0 >> saklay�c� bo�                           
         goto tekrar1;
      }                              // TMRT=1 >> saklay�c� dolu
      if(bit_test(PORTA,1)==1)       // bas�l� m�?
      {                              // bas�l� de�il
         TXREG = 0x00;               // o zaman s�f�r bilgisini g�nder
      }
   }                                 // bas�l� veri g�nderimi i�in tekrar kontrol et
   
// butona bi kere bas�nca veri gider ve TXREG de s�rekli kal�r
   if(bit_test(PORTA,0)==0)          // RA0 butona bas�l� m�? 
   {                                 // bas�l� burdan devam et        
      delay_ms(200);                 // butondan parmag� �ekme s�resi
      output_high(PIN_B2);           // hatt� 1'e �ek
      TXREG = 0x6C;                  // veriyi TXREG saklay�c�s�na y�kle
      tekrar:
      if(bit_test(TXSTA,1)==0)       // veri g�nderimi bitti mi   
      {                              // TMRT=0 >> saklay�c� bo�                           
         goto tekrar;
      }                              // TMRT=1 >> saklay�c� dolu                             
   }   
   goto bas;
}


