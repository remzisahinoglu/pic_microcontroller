//rs232 alýcý kesme uygulamasý
//29.03.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#use RS232(BAUD=19200, xmit=PIN_B2, rcv=PIN_B1)   // saniyede 1200 bit max gönder
#endif

#byte PORTA = 0x05
#byte PORTB = 0x06
#byte STATUS = 0x03
#byte OPTION = 0x81
#byte INTCON = 0x0B

char veri=0;

#int_RDA                   // usart kesmesi geldiðinde buradan basla
void RDA_isr()
{
   if(kbhit()==1)          // usar alýcý tampon kesme bayraðý biti
   {                       // bit 1 ise alýcý tampon dolu
      veri=getc();         // RXREG saklayýcýsýný oku veriye yaz
      output_b(veri);      // veri deki bilgiyi portb ye çak
      output_a(veri);      // veri deki bilgiyi portb ye çak
   }  
}

void main()
{
   set_tris_b(0b00000100);
   set_tris_a(0b00010000);      // mclr ve RA0-1 giriþ
   enable_interrupts(int_RDA);  // usart kesmesi aktif yapýlýr
   enable_interrupts(GLOBAL);   // kesmelere izin verilir
   
   while(TRUE)                  // normal program akýsý buradan olur
   {
      output_b(255);
      delay_ms(250);
      output_b(0);
      delay_ms(250);
   }
}
