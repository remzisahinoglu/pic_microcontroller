//get_fonk fon kullan�m� alc�� program� - 
//ba�ar�l� ama printf(lcd_putc) komutu datay� hem lcd ye hemde tx pininden g�nderiyor
//26.05.2011
//yazan = Remzi �AH�NO�LU

#if defined(__PCH__)
#include <18F4620.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <lcd416.c>                       // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd.c
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#include <stdlib.h>
#include <input.c>
#endif

//#device PIC18F4620

int degisim=0;
float m=0;

#int_rda
void serihaberlesme_kesmesi()          // \r=0D ENTER-SATIR BA�I, \n=0A SATIR ATLA
{  
   disable_interrupts(INT_RDA);
   m=get_float();
   output_high(PIN_C5);
   degisim = 1;
  

   output_low(PIN_C5);
}

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

   set_tris_c(0b10000000);
     
   lcd_init();
   
   lcd_putc("port dinleniyor");
     
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      enable_interrupts(INT_RDA);
      
      if(degisim == 1)
      {
         output_high(PIN_C4);
         degisim=0;
         lcd_send_byte(0,0x01);
         lcd_gotoxy(1,1);
         printf(lcd_putc, "%f" ,m);     // lcd ye g�nder
         
         delay_ms(2);

      }
   }
}
