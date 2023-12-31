// ana pic den al�nan veriyi 877 kullanarak lcd ekrana yaz

// 04.05.2011
// yazan = Remzi �AH�NO�LU

#if defined(__PCM__)
#include <16F877A.h>
#use delay(clock=4000000)
#fuses XT, NOWDT, NOPROTECT, NOLVP, NOBROWNOUT, NOPUT, NOWRT, NODEBUG, NOCPD
#define use_portb_lcd TRUE             // lcd bilgisi i�in port b�yi kullan�yoruz
#include <lcd416.c>                       // lcd i�in gerekecek fonksiyonlar�n bulundu�u dosya lcd.c
#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_c7, parity=N, stop=1)
#include <stdlib.h>
#include <input.c> 
#endif

//#device PIC16F877A

int i=0, degisim=0, a=2;
signed long long m=0;
long long t=0;

#int_rda
void serihaberlesme_kesmesi()          // \r=0D ENTER-SATIR BA�I, \n=0A SATIR ATLA
{  
   disable_interrupts(int_rda);
   output_high(PIN_C0);
   
   i = i+1;
   if(i==3){i=1;}
   if(i==1)
   {
      m=get_long();
   }
   
   if(i==2)
   {
      degisim = 1;
      t=get_long();
   }
   output_low(PIN_C0);
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
   
   output_a(0);
   output_low(PIN_C0);
   output_low(PIN_C1);
   output_low(PIN_C2);
   output_low(PIN_C3);
   output_low(PIN_C4);
   output_low(PIN_C5);
   
   enable_interrupts(GLOBAL);
   
   lcd_init();
   
   a=2;
   
   while(TRUE)
   {
      enable_interrupts(int_rda);
      //***** motor d�ng�s� **************************************************//
      if(degisim == 1)
      {
         degisim=0;
         disable_interrupts(int_rda);
         output_high(PIN_C3);             // i�lem yap�yor ledi aktif
         
         lcd_send_byte(0, 0x01);          // ekran� temizle
         lcd_gotoxy(1,1);
         printf(lcd_putc, "%ld", m);
         lcd_gotoxy(1,2);
         printf(lcd_putc, "%ld", t);
         
         if(m >= 0)
         {  
            while(m > 0)      // positive oldugu s�rece motor geri
            {
               m-=1;
               a+=1;                  // motor ileri d�n
               if(a>=5){a=1;}
               switch (a)
               {
                  case 1: output_a(1); delay_ms(t);  break;
                  case 2: output_a(2); delay_ms(t);  break;
                  case 3: output_a(4); delay_ms(t);  break;
                  case 4: output_a(8); delay_ms(t);  break;
               }
            }
         }
         else
         {
            while(m < 0)      // negative oldugu s�rece motor geri
            {
               m+=1;
               a-=1;                  // motor geri d�n
               if(a<=0){a=4;}
               switch (a)
               {
                  case 1: output_a(1); delay_ms(t);  break;
                  case 2: output_a(2); delay_ms(t);  break;
                  case 3: output_a(4); delay_ms(t);  break;
                  case 4: output_a(8); delay_ms(t);  break;
               }           
            }
         }
         //printf("%ld\n\r", m);
         output_low(PIN_C3);           // i�lem bitti ledi pasif
      }
      
   }
}
