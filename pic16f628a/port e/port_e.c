// delta robot i�in test program� step motor kontrol program� 
// 4 bit ile s�rayla 3 step motor kontrol� cmos 4067 entegresi ile

// port e sadece c�k�s olarak kullan�l�yor.

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
   

   
   lcd_init();
   
   while(TRUE)
   {
      if(input(PIN_E0)==0)
      {
         output_d(0xFF);  
      }
      if(input(PIN_E1)==0)
      {
         output_d(0x00);  
      }
   }
}
