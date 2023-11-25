// float turunden bir say�y� eproma yazd�rma

// yazan: Remzi �ahino�lu
// tarih: 03.05.2011

#if defined(__PCH__)
#include <18F46K20.h>
#include <MATH.h> 
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#include <stdlib.h>        // bu s�ralama dogru rs232 den altta olmal�
#include <input.c>
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <LCD416.c>                    // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd416.c
#endif

//#device PIC18F46K20

float x,y;


void ee_write_float(unsigned int addr , float *floatptr)
{
unsigned char edata;
unsigned char I;
    for(I=0;I<4;I++){
      edata = *((unsigned char *)floatptr+I);
      write_eeprom(addr+I,edata);
   }
}

void ee_read_float(unsigned int addr , float *floatptr)
{
unsigned char edata;
unsigned char I;
   for(I=0;I<4;I++){
      edata=read_eeprom(I+addr);   
        *((unsigned char *)floatptr+I) = edata;
   }
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
   
   set_tris_d(0xFF);

   
   lcd_init();    // lcdyi haz�rla

   while(TRUE)
   {
      if(input(PIN_D0)==0)
      {
         delay_ms(100);
         x=30.152454;
         ee_write_float(0x00, &x);
      }
      
      if(input(PIN_D1)==0)
      {
         delay_ms(100);
         
         lcd_gotoxy(1,1);
         ee_read_float(0x00, &y);
         printf(lcd_putc,"%4.6f",y);
      }
      
      if(input(PIN_D2)==0)
      {
         delay_ms(100);
         x = -12.000595;
         ee_write_float(0x04, &x);

      }
      if(input(PIN_D3)==0)
      {
         delay_ms(100);
         
         lcd_gotoxy(1,2);
         ee_read_float(0x04, &y);
         printf(lcd_putc,"%4.6f",y);
      }     
   }
}

