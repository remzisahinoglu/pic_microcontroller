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

void main()
{
   char veri=0;   

   do                                  // // enerjiyi kesseniz dahi bilgi silme komutu gelemdi�i taktirde adreste kalacakt�r
   {
      if(bit_test(PORTA,0)==0)         // veriyi yaz
      {write_eeprom(0x00, 0x0F);}      // 0x00 adresine 0x0F bilgisini yaz
                                                      
      if(bit_test(PORTA,1)==0)         // veriyi yaz
      {write_eeprom(0x7F, 0x01);}      // 0x7F adresine 0x01 bilgisini yaz
      
      if(bit_test(PORTA,2)==0)         // veriyi oku
      {
         veri = read_eeprom (0x00);    // 0x00 adresini oku      ilk adres 0.byte
         output_b(veri);               // ve portb ye g�nder
      }
      if(bit_test(PORTA,3)==0)         // veriyi oku
      {
         veri = read_eeprom (0x7F);    // 0x7F adresini oku      son adres 128.byte
         output_b(veri);               // ve portb ye g�nder
      }
   }while(TRUE);                       // sonsuz d�g�
}

