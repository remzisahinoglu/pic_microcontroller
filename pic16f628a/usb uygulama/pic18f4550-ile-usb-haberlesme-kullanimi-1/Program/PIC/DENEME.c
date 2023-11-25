
#include <18F4550.h>
#device ADC=10
#fuses HSPLL,USBDIV,PLL5,PUT,CPUDIV1,VREGEN,NOWDT,NOPROTECT,NOLVP,NODEBUG,NOMCLR
#use delay(clock=48000000)

#define use_portb_lcd True
#define USB_HID_DEVICE     TRUE             
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT //Uçnokta1'de Kesme transferi aktif
#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT    
#define USB_EP1_TX_SIZE    64                 //Uçnokta1 için maksimum alýnacak ve gonderilecek
#define USB_EP1_RX_SIZE    64                 //veri boyutu (64 byte)

#include <benim_lcd.c>
#include <pic18_usb.h>     
#include <USB_Konfigurasyon.h>                //USB konfigurasyon bilgileri bu dosyadadýr.
#include <usb.c>   


#define UcNokta1       1 
#define Komut          gelen_paket[0]

//Yazýlým (Firmware) Sürüm no
#define surum_no1      0x00            //surum_no2.surum_no1 ornek: 1.0
#define surum_no2      0x01 

//Komutlar
#define LED_Kontrol    0x04
#define LCD_Yaz        0x05

void user_init(void)
{

   set_tris_a(0xff);                   //PORTA GÝRÝÞ
   set_tris_b(0x00);                   //PORTB ÇIKIÞ
   set_tris_c(0x07);                   //PORTC ÝLK 3 PÝN ÇIKIÞ DÝÐERLERÝ GÝRÝÞ
   set_tris_d(0x00);                   //PORTD ÇIKIÞ
   set_tris_e(0x00);                   //PORTE ÇIKIÞ      
   output_d(0);
   setup_adc_ports(ADC_OFF );
   lcd_hazirla();
}

void LCD_MetinYaz(char yazi[])
{
   unsigned char i;
   unsigned char k=0;
   char harf;
   
   printf(lcd_veri,"\f");
   
   ++yazi;
   i=*yazi++;
   harf=*yazi++; 
   lcd_komut(0x01);
   for(;i!=0;i--)
   {        
      printf(lcd_veri,"%c",harf);
      delay_us(5);
      
      if(k==15)
         imlec(2,1);
         
      harf=*yazi++;
      k++;
   } 
}

void main(void)
{
   byte sayac=0;
   byte gelen_paket[65];               //gelen paket
   byte gond_paket[65];                //gönderilecek paket
 
 //Sistemi baþlat
   user_init();                     
   usb_init();                      
   usb_task();  
   printf(lcd_veri,"USB BAGLANTISI");//USB baðlantýsý kurulduysa LCD'de göster
   imlec(2,1);
   printf(lcd_veri,"KURULUYOR...");//USB baðlantýsý kurulduysa LCD'de göster
   usb_wait_for_enumeration();         //Cihaz, hazýr olana kadar bekle
   
   if(usb_enumerated())
   lcd_komut(0x01);
   imlec(1,1);
   printf(lcd_veri,"USB BAGLANTISI");//USB baðlantýsý kurulduysa LCD'de göster
   imlec(2,1);
   printf(lcd_veri,"KURULDU");//USB baðlantýsý kurulduysa LCD'de göster
   
 
   for (;;)
   {
      while(usb_enumerated())
      {
  
         if (usb_kbhit(1))             //Eðer pc'den yeni bir paket geldiyse
         {
          
            usb_get_packet(UcNokta1, gelen_paket, 65); //paketi oku
            
            switch(Komut)              // Paketin ilk elemanýndaki komutu oku ve ilgili göreve git
               {
                  case LED_Kontrol:                         
                           gond_paket[0]=0;
                           output_c(gelen_paket[1]<<3/10);
                           output_d(gelen_paket[1]>>3);                          
                           lcd_komut(0x01);
                           imlec(1,1);
                           printf(lcd_veri,"LED=>0x%X",gelen_paket[1]);
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

