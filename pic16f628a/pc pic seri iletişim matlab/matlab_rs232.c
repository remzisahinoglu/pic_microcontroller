//matlabdan al�nan veriyi 877 kullanarak lcd ekrana yaz
//20.04.2011
//yazan = Remzi �AH�NO�LU

#if defined(__PCM__)
#include <16F877A.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <lcd.c>                       // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd.c
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#endif

char veri[80];

#int_rda
void serihaberlesme_kesmesi()
{
   disable_interrupts(int_rda);
   output_high(pin_c5);
   //veri=getc();
   gets(veri);
   //lcd_putc(veri);
   printf(lcd_putc,"\f%s", veri);      // \r=0D ENTER-SATIR BA�I, \n=0A SATIR ATLA
   output_low(pin_c5);
   
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
   
   output_low(pin_c5);
   lcd_init();
   
   lcd_putc("port dinleniyor");
   
   //printf("\n\rKlavyeden bir metin giriniz ve enter tusuna basiniz>");
   
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      enable_interrupts(int_rda);
   
   }
}

