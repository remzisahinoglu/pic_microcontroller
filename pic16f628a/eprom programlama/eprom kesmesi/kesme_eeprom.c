//eeprom veri yazma ve okuma uygulams�
//22.04.2007
//yazan = Remzi �AH�NO�LU
// pic 16f628a' n�n 128byte l�k eepr�m veri belle�i vard�r
#if defined (__PCM__)
#include <16F628A.h>
#use delay(clock=4000000)
#fuses NOWDT,PUT,NOLVP,NOBROWNOUT,MCLR,INTRC_IO,NOPROTECT
#endif

#byte PORTA = 0x05
#byte PORTB = 0x06

char veri=0, say=0, artma=500;

#int_eeprom         // eproma yazma tamamland��� zaman program ak��� buradan devam eder
void eprom_yaz()
{
   artma=artma-10;   // sayma h�z�n� artt�r ge�ikem s�resini azald��� i�in
}
 
void main()
{
   enable_interrupts(global);
   enable_interrupts(int_eeprom);
   
   do
   {
      say=say+1;                       // normal program ak���
      delay_ms(artma);
      output_b(say);
      
      
      if(bit_test(PORTA,0)==0)         // veriyi yaz
      {write_eeprom(0x00, 0x0F);}      // 0x00 adresine 0x0F bilgisini yaz
                                       // enerjiyi kesseniz dahi bilgi silme komutu gelemdi�i taktirdeadreste kalacakt�r
      
      if(bit_test(PORTA,2)==0)         // veriyi oku
      {
         veri = read_eeprom (0x00);    // 0x00 adresini oku      ilk adres 0.byte
         output_b(veri);               // ve portb ye g�nder
      }
      
   }while(TRUE);                       // sonsuz d�g�
}

