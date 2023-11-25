//rs232 al�c� uygulams�
//01.03.2007
//yazan = Remzi �AH�NO�LU
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#use  RS232(BAUD=1200, xmit=PIN_B2, rcv=PIN_B1)   // saniyede 1200 bit max al
#endif

#bit PIR1 = 0x0C.5
#byte RCREG = 0x1A
#byte veri = 0

void main()
{
   set_tris_a(0b00100000);    // mclr giri� (reset var) ilk 4 bit ��k�s
   set_tris_b(0b00000010);    // rcv giri� 
   delay_ms(500);
   
   output_a(0);     
   output_b(0);   
   veri=0;
   
   while(TRUE)                // sonsuz d�ng�
   {
      if(PIR1==1)             // usart al�c� tampon kesme bayra�� biti
      {                       // bit 1 ise al�c� tampon dolu
         veri=RCREG;          // RCREG saklay�c�s�n� oku veriye yaz      
      }
      
      output_a(veri);         // A portundan direk ��kart      
   }
}


