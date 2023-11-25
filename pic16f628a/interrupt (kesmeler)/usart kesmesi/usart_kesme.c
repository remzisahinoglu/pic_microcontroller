//rs232 al�c� kesme uygulamas�
//29.03.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#use RS232(BAUD=19200, xmit=PIN_B2, rcv=PIN_B1)   // saniyede 1200 bit max g�nder
#endif

#byte PORTA = 0x05
#byte PORTB = 0x06
#byte STATUS = 0x03
#byte OPTION = 0x81
#byte INTCON = 0x0B

char veri=0;

#int_RDA                   // usart kesmesi geldi�inde buradan basla
void RDA_isr()
{
   if(kbhit()==1)          // usar al�c� tampon kesme bayra�� biti
   {                       // bit 1 ise al�c� tampon dolu
      veri=getc();         // RXREG saklay�c�s�n� oku veriye yaz
      output_b(veri);      // veri deki bilgiyi portb ye �ak
      output_a(veri);      // veri deki bilgiyi portb ye �ak
   }  
}

void main()
{
   set_tris_b(0b00000100);
   set_tris_a(0b00010000);      // mclr ve RA0-1 giri�
   enable_interrupts(int_RDA);  // usart kesmesi aktif yap�l�r
   enable_interrupts(GLOBAL);   // kesmelere izin verilir
   
   while(TRUE)                  // normal program ak�s� buradan olur
   {
      output_b(255);
      delay_ms(250);
      output_b(0);
      delay_ms(250);
   }
}
