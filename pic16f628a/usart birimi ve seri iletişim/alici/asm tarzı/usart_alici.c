//usart birimi seri ileti�im al�c� uygulamas�
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
#byte RCREG = 0x1A
#byte PIR1 = 0x0C

void main()
{
   char veri=0;
   
   set_tris_a(0b11110000);           // Porta n�n ilk4 biti ve portb nin son4 biti ��k�� 
   set_tris_b(0b00000010);           // RB1/RX=giri�, RB2/TX=��k��
   SPRG = 25;                        // bound h�z� 9600 olara kayarland� 
   TXSTA = 0b00000100;               // BRGH=1 (y�ksek h�z aktif)
   RCSTA = 0b10010000;               // SPEN=1 (seri port aktif)
                                     // CREN=1 (s�rekli veri al�m� aktif)                                     
// veri okuma rutinleri
tekrar:
   if (bit_test(PIR1,5)==1)          // RCIF al�c� tampon bo� mu?
   {                                 // gelen veri tamponda, veriyi porta ya yaz
      veri = RCREG;                  //
      output_a(veri);                // gelen veriyi portlara yaz
      output_b(veri);                //  
   }                                 // gelen veri daha tampona aktar�lmad�, tekrar kontrol et
   goto tekrar;
}

