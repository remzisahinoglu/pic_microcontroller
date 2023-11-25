//get_long fon kullanýmý alcýý programý - 
//baþarýlý ama printf(lcd_putc) komutu datayý hem lcd ye hemde tx pininden gönderiyor
//01.05.2011
//yazan = Remzi ÞAHÝNOÐLU

#if defined(__PCM__)
#include <16F877A.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portb_lcd TRUE             // lcd bilgisi için port b’yi kullanýyoruz
#include <lcd416.c>                       // lcd için gerekecek fonksiyonlarýn bulunduðu dosya lcd.c
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#include <stdlib.h>
#include <input.c> 
#endif

//#device PIC16F877

int i=0, degisim=0;
signed long m=0;
signed long t=0;

#int_rda
void serihaberlesme_kesmesi()          // \r=0D ENTER-SATIR BAÞI, \n=0A SATIR ATLA
{  
   disable_interrupts(int_rda);
   output_high(pin_c5);
   
   i = i+1;
   if(i==3){i=1;}
   if(i==1)
   {
      m=get_long();
      output_high(PIN_C0);
   }
   
   if(i==2)
   {
      degisim = 1;
      output_high(PIN_C1);
      t=get_long();
   }
   output_low(pin_c5);
   output_low(pin_c0);
   output_low(pin_c1);
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

   
   output_low(pin_c5);
   lcd_init();
   
   lcd_putc("port dinleniyor");
     
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      enable_interrupts(int_rda);
      
      if(degisim == 1)
      {
         degisim=0;
         lcd_send_byte(0,0x01);
         lcd_gotoxy(1,1);
         printf(lcd_putc, "%ld" , m);     // lcd ye gönder
         
         delay_ms(2);
         
         lcd_gotoxy(1,2);
         printf(lcd_putc, "%ld" , t);     // lcd ye gönder
         
         delay_ms(2);
      }
   }
}
