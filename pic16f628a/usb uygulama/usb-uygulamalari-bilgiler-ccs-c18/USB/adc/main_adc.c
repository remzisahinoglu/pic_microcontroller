
#include <18F2550.h>
#device ADC=10
#fuses HSPLL,USBDIV,PLL5,PUT,CPUDIV1,VREGEN,NOWDT,NOPROTECT,NOLVP,NODEBUG,NOMCLR
#use delay(clock=48000000)

#define USB_HID_DEVICE     FALSE             
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK    //U�nokta1'de Y���n transferi aktif
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK    
#define USB_EP1_TX_SIZE    64                 //U�nokta1 i�in maksimum al�nacak ve gonderilecek
#define USB_EP1_RX_SIZE    64                 //veri boyutu (64 byte)

#include <pic18_usb.h>     
#include <USB_Konfigurasyon.h>                //USB konfigurasyon bilgileri bu dosyadad�r.
#include <usb.c>          

#define UcNokta1       1 
#define Komut          gelen_paket[0]

//Yaz�l�m (Firmware) S�r�m no
#define surum_no1      0x00            //surum_no2.surum_no1 ornek: 1.0
#define surum_no2      0x01 

//Komutlar
#define surum_oku      0x03
#define olcum_al       0x02

static unsigned int16 ADC_deger1 = 0;
static unsigned int16 ADC_deger2 = 0;
    
void user_init(void)
{
   set_tris_a(0x03);                    //RA0,RA1 giri�
   set_tris_b(0xFF);
   setup_adc_ports(AN0_TO_AN1);
   setup_adc(ADC_CLOCK_INTERNAL);
 
}
unsigned int16 ADC_Oku(byte kanal)
{
   unsigned int16 olcum=0;
   set_adc_channel(kanal);
   delay_us(20);
   olcum = read_adc();
   return olcum;
}

void main(void)
{
   byte sayac=0;
   byte gelen_paket[64];               //gelen paket
   byte gond_paket[64];                //g�nderilecek paket
   byte surum1=surum_no1;
   byte surum2=surum_no2;
 //Sistemi ba�lat
   user_init();                     
   usb_init();                      
   usb_task();                      

   usb_wait_for_enumeration();         //Cihaz, haz�r olana kadar bekle
   if(usb_enumerated())
   output_high(Pin_A2);                //USB ba�lant�s� kurulduysa LED'i yak.

   for (;;)
   {
      while(usb_enumerated())
      {
         if (usb_kbhit(1))             //E�er pc'den yeni bir paket geldiyse
         {
            usb_get_packet(UcNokta1, gelen_paket, 64); //paketi oku
            switch(Komut)              // Paketin ilk eleman�ndaki komutu oku ve ilgili g�reve git
               {
               
                  case surum_oku:
                           gond_paket[0] =  Komut;
                           gond_paket[1] =  surum1;  
                           gond_paket[2] =  surum2;
                           sayac = 0x03;
                           break;
                           
                  case olcum_al:                      
                           ADC_deger1=ADC_Oku(0);
                           ADC_deger2=ADC_Oku(1);
                           gond_paket[0] =  Komut;
                           gond_paket[1] = (byte) ADC_deger1;
                           gond_paket[2] = (byte)(ADC_deger1>>8);
                           gond_paket[3] = (byte) ADC_deger2;
                           gond_paket[4] = (byte)(ADC_deger2>>8);                          
                           sayac = 0x05;
                           break;               
                                        
                  default: ; break;
                           
               }
         }
         if(sayac!=0)
         {
         usb_put_packet(UcNokta1, gond_paket, sayac, USB_DTS_TOGGLE);
         sayac = 0;
         Komut = 0;
         }
      }

   }
}

