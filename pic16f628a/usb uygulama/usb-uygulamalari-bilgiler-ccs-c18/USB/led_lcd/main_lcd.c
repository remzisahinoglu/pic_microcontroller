
#include <18F4550.h>
#device ADC=10
#fuses HSPLL,USBDIV,PLL5,PUT,CPUDIV1,VREGEN,NOWDT,NOPROTECT,NOLVP,NODEBUG,NOMCLR
#use delay(clock=48000000)

#define use_portb_lcd True
#define USB_HID_DEVICE     TRUE             
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT //U�nokta1'de Kesme transferi aktif
#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT    
#define USB_EP1_TX_SIZE    64                 //U�nokta1 i�in maksimum al�nacak ve gonderilecek
#define USB_EP1_RX_SIZE    64                 //veri boyutu (64 byte)

#include <lcd.c>
#include <pic18_usb.h>     
#include <USB_Konfigurasyon.h>                //USB konfigurasyon bilgileri bu dosyadad�r.
#include <usb.c>   


#define UcNokta1       1 
#define Komut          gelen_paket[0]

//Yaz�l�m (Firmware) S�r�m no
#define surum_no1      0x00            //surum_no2.surum_no1 ornek: 1.0
#define surum_no2      0x01 

//Komutlar
#define LED_Kontrol    0x04
#define LCD_Yaz        0x05

   
void user_init(void)
{
   set_tris_a(0xFF);                    
   set_tris_b(0x00);
   set_tris_d(0x00);
   output_d(0);
   setup_adc_ports(NO_ANALOGS );
   lcd_init();
 
}
void LCD_MetinYaz(char yazi[])
{
   unsigned char i;
   unsigned char k=0;
   char harf;
   
   printf(lcd_putc,"\f");
   
   ++yazi;
   i=*yazi++;
   harf=*yazi++; 
   
   for(;i!=0;i--)
   {        
      printf(lcd_putc,"%c",harf);
      delay_us(5);
      
      if(k==15)
         printf(lcd_putc,"\n");
         
      harf=*yazi++;
      k++;
   } 
}

void main(void)
{
   byte sayac=0;
   byte gelen_paket[65];               //gelen paket
   byte gond_paket[65];                //g�nderilecek paket
 
 //Sistemi ba�lat
   user_init();                     
   usb_init();                      
   usb_task();                      
   usb_wait_for_enumeration();         //Cihaz, haz�r olana kadar bekle
   
   if(usb_enumerated())
      printf(lcd_putc,"\f USB baglantisi \n     kuruldu");//USB ba�lant�s� kurulduysa LCD'de g�ster

   for (;;)
   {
      while(usb_enumerated())
      {
  
         if (usb_kbhit(1))             //E�er pc'den yeni bir paket geldiyse
         {
          
            usb_get_packet(UcNokta1, gelen_paket, 65); //paketi oku
            
            switch(Komut)              // Paketin ilk eleman�ndaki komutu oku ve ilgili g�reve git
               {
                  case LED_Kontrol:                         
                           gond_paket[0]=0;                           
                           output_d(gelen_paket[1]);
                           printf(lcd_putc,"\f 0x%X",gelen_paket[1]);
                           sayac = 0x01;
                           Komut = 0;
                           break;
                
                  case LCD_Yaz:
                           LCD_MetinYaz(gelen_paket);                                                   
                           sayac = 0x00;
                           break;
                           
                  default: ; break;
                           
               }
         }
         if(sayac!=0)      
         {           
            if(usb_put_packet(UcNokta1, gond_paket, 65, USB_DTS_TOGGLE))
            {
               sayac = 0;
               Komut = 0;
            }
         }

      }

   }
}

