//rs232 verici uygulamasý
//27.03.2007
//yazan = Remzi ÞAHÝNOÐLU
#if defined (__PCM__)
#include <16F877A.h>
#use delay(clock=4000000)
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#use RS232(BAUD=9600, xmit=PIN_B2, rcv=PIN_B1)
#endif

#byte PORTA = 0x05

void main()
{
   set_tris_b(0b00000100);
   set_tris_a(0b00010011);      // mclr ve RA0-1 giriþ
   while(TRUE)                  // sonsuz döngü
   {
      if(bit_test(PORTA,0)==0)  // butonu kontrol et
      {         
         putc(10);              // 10 decimal deðerini seri olarak gönder     
      }      
      if(bit_test(PORTA,1)==0)
      {         
         putc(01);              // 01 decimal degerini seri olarak gönder         
      }
   }
}
