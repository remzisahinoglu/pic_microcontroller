//power butonu ile çok durumlu flosr uygulamasý
//28.06.2012
//yazan = Remzi ÞAHÝNOÐLU

#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,NOMCLR,INTRC_IO,NOPROTECT
#endif

char veri=0;

void main()
{ 
   delay_ms(200);
   output_b(0);
   output_a(0);
   
   veri = read_eeprom(0);        // eprom belleginin 0. adresini oku
   veri=veri+1;
   if(veri > 5){veri=1;}
   write_eeprom(0, veri);        // eprom belleðinin 0. adresine veriyi yaz
   
   while(TRUE)
   {
      switch(veri)
      {
         case 1:  output_high(PIN_B3);           // sürekli yan
                  break;
                                                    
         case 2:  output_high(PIN_B3);           // kesikli yan
                  delay_ms(40);
                  output_low(PIN_B3);
                  delay_ms(60);
                  
                  output_high(PIN_B3);
                  delay_ms(40);
                  output_low(PIN_B3);
                  delay_ms(60);
                  
                  output_high(PIN_B3);
                  delay_ms(40);
                  output_low(PIN_B3);
                  delay_ms(500);
                  break;
         
         case 3:  output_high(PIN_B3);           // kesikli yan-kýsa
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(500);
                  break;
                  
         case 4:  output_high(PIN_B3);
                  delay_ms(100);
                  output_low(PIN_B3);
                  delay_ms(200);
                  
                  output_high(PIN_B3);
                  delay_ms(100);
                  output_low(PIN_B3);
                  delay_ms(200);
                  
                  output_high(PIN_B3);
                  delay_ms(100);
                  output_low(PIN_B3);
                  delay_ms(200);
                  break;
                  
         case 5:  output_high(PIN_B3);           // S.O.S (save our souls)
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(50);
                  output_high(PIN_B3);
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(50);
                  output_high(PIN_B3);
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(50);
                                  
                  output_high(PIN_B3);
                  delay_ms(150);
                  output_low(PIN_B3);
                  delay_ms(150);
                  output_high(PIN_B3);
                  delay_ms(150);
                  output_low(PIN_B3);
                  delay_ms(150);
                  output_high(PIN_B3);
                  delay_ms(150);
                  output_low(PIN_B3);
                  delay_ms(150);
                  
                  output_high(PIN_B3);
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(50);
                  output_high(PIN_B3);
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(50);
                  output_high(PIN_B3);
                  delay_ms(50);
                  output_low(PIN_B3);
                  delay_ms(1000);
                  
                  break;
      }
   }
}
