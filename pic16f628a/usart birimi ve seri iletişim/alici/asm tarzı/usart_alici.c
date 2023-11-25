//usart birimi seri iletiþim alýcý uygulamasý
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
#byte RCREG = 0x1A
#byte PIR1 = 0x0C

void main()
{
   char veri=0;
   
   set_tris_a(0b11110000);           // Porta nýn ilk4 biti ve portb nin son4 biti çýkýþ 
   set_tris_b(0b00000010);           // RB1/RX=giriþ, RB2/TX=çýkýþ
   SPRG = 25;                        // bound hýzý 9600 olara kayarlandý 
   TXSTA = 0b00000100;               // BRGH=1 (yüksek hýz aktif)
   RCSTA = 0b10010000;               // SPEN=1 (seri port aktif)
                                     // CREN=1 (sürekli veri alýmý aktif)                                     
// veri okuma rutinleri
tekrar:
   if (bit_test(PIR1,5)==1)          // RCIF alýcý tampon boþ mu?
   {                                 // gelen veri tamponda, veriyi porta ya yaz
      veri = RCREG;                  //
      output_a(veri);                // gelen veriyi portlara yaz
      output_b(veri);                //  
   }                                 // gelen veri daha tampona aktarýlmadý, tekrar kontrol et
   goto tekrar;
}

