//eeprom veri yazma ve okuma uygulamsý
//22.04.2007
//yazan = Remzi ÞAHÝNOÐLU
// pic 16f628a' nýn 128byte lýk eeprým veri belleði vardýr
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
#byte PORTB = 0x06

char veri=0, say=0, artma=500;

#int_eeprom         // eproma yazma tamamlandýðý zaman program akýþý buradan devam eder
void eprom_yaz()
{
   artma=artma-10;   // sayma hýzýný arttýr geçikem süresini azaldýðý için
}
 
void main()
{
   enable_interrupts(global);
   enable_interrupts(int_eeprom);
   
   do
   {
      say=say+1;                       // normal program akýþý
      delay_ms(artma);
      output_b(say);
      
      
      if(bit_test(PORTA,0)==0)         // veriyi yaz
      {write_eeprom(0x00, 0x0F);}      // 0x00 adresine 0x0F bilgisini yaz
                                       // enerjiyi kesseniz dahi bilgi silme komutu gelemdiði taktirdeadreste kalacaktýr
      
      if(bit_test(PORTA,2)==0)         // veriyi oku
      {
         veri = read_eeprom (0x00);    // 0x00 adresini oku      ilk adres 0.byte
         output_b(veri);               // ve portb ye gönder
      }
      
   }while(TRUE);                       // sonsuz dögü
}

