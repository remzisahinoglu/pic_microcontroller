// delta robot i�in test program� step motor kontrol program� 
// 4 bit ile s�rayla 3 step motor kontrol� cmos 4067 entegresi ile

//20.04.2011
//yazan = Remzi �AH�NO�LU

#if defined(__PCH__)
#include <18F46K20.h>
#include <MATH.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portd_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <LCD416.c>                    // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd416.c
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#endif



void main()
{
   setup_PSP(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   
   set_tris_c(0b10111111);
   
   
   output_d(0);
   while(TRUE)
   {
      output_d(0b10000000);      // motorlar stop LED aktif
      delay_ms(2000);
      
      // step motor 1 i�in 4 ad�m ileri ve MA on LED aktif
      output_d(0b00010001);   // hex: 0x11
      delay_ms(500);
      output_d(0b00010010);   // hex: 0x12
      delay_ms(500);
      output_d(0b00010011);   // hex: 0x13
      delay_ms(500);
      output_d(0b00010100);   // hex: 0x14
      delay_ms(500);
      
      // step motor 2 i�in 4 ad�m ileri ve MB on LED aktif
      output_d(0b00100101);   // hex: 0x25
      delay_ms(500);
      output_d(0b00100110);   // hex: 0x26
      delay_ms(500);
      output_d(0b00100111);   // hex: 0x27
      delay_ms(500);
      output_d(0b00101000);   // hex: 0x28
      delay_ms(500);
      
      // step motor 3 i�in 4 ad�m ileri ve MC on LED aktif
      output_d(0b01001001);   // hex: 0x49
      delay_ms(500);
      output_d(0b01001010);   // hex: 0x4A
      delay_ms(500);
      output_d(0b01001011);   // hex: 0x4B
      delay_ms(500);
      output_d(0b01001100);   // hex: 0x4C
      delay_ms(500);
      
//*************** tek ad�m kontrol 3 motor k�smen e�zamanl� ******************//
      output_d(0b10000000);      // motorlar stop LED aktif
      delay_ms(2000);
      
      // ilk ad�mlar
      output_d(0b00010001);   // hex: 0x11
      delay_ms(10);                          // motora g�re 10ms yeterli olmayabilir.
      output_d(0b00100101);   // hex: 0x25
      delay_ms(10);
      output_d(0b01001001);   // hex: 0x49
      delay_ms(500);
      
      // ikinci ad�mlar
      output_d(0b00010010);   // hex: 0x12
      delay_ms(10);
      output_d(0b00100110);   // hex: 0x26
      delay_ms(10);
      output_d(0b01001010);   // hex: 0x4A
      delay_ms(500);
      
      // ���nc� ad�mlar
      output_d(0b00010011);   // hex: 0x13
      delay_ms(10);
      output_d(0b00100111);   // hex: 0x27
      delay_ms(10);
      output_d(0b01001011);   // hex: 0x4B
      delay_ms(500);
      
      // d�rd�nc� ad�mlar
      output_d(0b00010100);   // hex: 0x14
      delay_ms(10);
      output_d(0b00101000);   // hex: 0x28
      delay_ms(10);
      output_d(0b01001100);   // hex: 0x4C
      delay_ms(500);
   }

}
