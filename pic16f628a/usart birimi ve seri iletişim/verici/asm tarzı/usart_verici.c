//usart birimi seri iletiþim verici uygulamasý
//21.02.2007
//yazan = Remzi ÞAHÝNOÐLU
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
   set_tris_b(0b00000010);           // RB1/RX=giriþ, RB2/TX=çýkýþ
   SPRG = 25;                        // bound hýzý 9600 olara kayarlandý     
   TXSTA = 0b00100100;               // BRGH=1(yüksek hýz), TXEN=1(gönderme aktif)
   bit_set(RCSTA,7);                 // seri iletiþim aktif (SPEN)
   
   delay_ms(100);                    // biraz bekle acele etme :)
// veri gönderme rutinleri
bas:   
// butona basýlý tutunca veri gönderilir, parmak butondan çekildiðinde gönderilen veri tekrar sýfýr olur         
   if(bit_test(PORTA,1)==0)          // RA1 butona basýlý mý?
   {                                 // basýlý burdan devam et
      delay_ms(200);                 // butondan parmagý çekme süresi
      output_high(PIN_B2);           // hattý 1'e çek
      TXREG = 0x6A;                  // veriyi TXREG saklayýcýsýna yükle
      tekrar1:
      if(bit_test(TXSTA,1)==0)       // veri gönderimi bitti mi   
      {                              // TMRT=0 >> saklayýcý boþ                           
         goto tekrar1;
      }                              // TMRT=1 >> saklayýcý dolu
      if(bit_test(PORTA,1)==1)       // basýlý mý?
      {                              // basýlý deðil
         TXREG = 0x00;               // o zaman sýfýr bilgisini gönder
      }
   }                                 // basýlý veri gönderimi için tekrar kontrol et
   
// butona bi kere basýnca veri gider ve TXREG de sürekli kalýr
   if(bit_test(PORTA,0)==0)          // RA0 butona basýlý mý? 
   {                                 // basýlý burdan devam et        
      delay_ms(200);                 // butondan parmagý çekme süresi
      output_high(PIN_B2);           // hattý 1'e çek
      TXREG = 0x6C;                  // veriyi TXREG saklayýcýsýna yükle
      tekrar:
      if(bit_test(TXSTA,1)==0)       // veri gönderimi bitti mi   
      {                              // TMRT=0 >> saklayýcý boþ                           
         goto tekrar;
      }                              // TMRT=1 >> saklayýcý dolu                             
   }   
   goto bas;
}


