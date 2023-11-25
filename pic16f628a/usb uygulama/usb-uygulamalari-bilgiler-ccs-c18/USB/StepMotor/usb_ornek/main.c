#include <p18cxxx.h>
#include "..\usb\typedefs.h"
#include "..\usb\usb.h" 
#include "io_cfg.h" 
#include <adc.h>
#include <delays.h>        
 
   
//-----------------------------
#pragma config CPUDIV = OSC1_PLL2,PLLDIV = 5,USBDIV = 2
#pragma config FOSC = HSPLL_HS,IESO = OFF
#pragma config PWRT = OFF,BOR = OFF,VREGEN = ON
#pragma config WDT = OFF,MCLRE = OFF,LPT1OSC = OFF
#pragma config PBADEN = OFF,LVP = OFF

#define Komut        gelen_paket[0]
#define dur			 0
#define sag          1
#define sol			 2
#define motorPort    PORTB

//Yazýlým (Firmware) Sürüm no
#define surum_no1      0x00            //surum_no2.surum_no1 ornek: 1.0
#define surum_no2      0x01 

//USB Komutlarý
#define olcum_al       0x02
#define surum_oku      0x03
#define motor_sur	   0x04

//Genel Degiskenler
byte gelen_paket[USBGEN_EP_SIZE]; 
byte gond_paket[USBGEN_EP_SIZE]; 

static unsigned int ADC_deger1=0;		//Olcum sonuclarýný tutan degiskenler
static unsigned int ADC_deger2=0;
static byte Adim = 0;
static byte Yon = 0;
static byte Motor = 0;
static byte Hiz = 0;
static BOOL ZamanAyarlimi = FALSE;
static byte adim_durum = 0;
byte const durumlar[4] = {0x22,0x88,0x44,0x11}; //Tam Adým bipolar stepmotor

//Fonksiyon Prototipleri	 
static void InitializeSystem(void);
void USBTasks(void);
void ADC_Oku(void);
void StepmotorSur(byte nHiz, byte nYon, byte nMotor);


#pragma code
void main(void)
{
	byte surum1 = surum_no1;
	byte surum2 = surum_no2;
	byte sayac = 0;
   
    
	InitializeSystem();
    
    while(1)
    {
        USBTasks();  //Usb baðlantý durumunu takip eder      
        
        if (!(usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1))
        {   
	        PORTAbits.RA2=1; //USB baðlantýsý aktif ise LED'i yak 
    		if(USBGenRead(gelen_paket,64))  // Gelen paketi oku (byte*)&paket,sizeof(paket))
    		{
        		
        		switch(Komut)
       			 {
	       			 
		            case surum_oku:
		            	 gond_paket[0] = gelen_paket[0];      		  
		                 gond_paket[1] = surum1;  
		                 gond_paket[2] = surum2;  
		                 sayac = 0x03;
		                 break;		             
		            case olcum_al:
		            	 ADC_Oku(); 		            	 
		                 gond_paket[1] = (byte) ADC_deger1 ;
		                 gond_paket[2] = (byte)(ADC_deger1>>8);
		                 gond_paket[3] = (byte) ADC_deger2 ;
		                 gond_paket[4] = (byte)(ADC_deger2>>8);
		                 sayac = 0x05;
		            	 break;
		            case motor_sur: 
		            	 gond_paket[0] = gelen_paket[0];           	 
		                 Adim = gelen_paket[1];
		                 Hiz  = gelen_paket[2];
		                 Yon  = gelen_paket[3];
		                 Motor = gelen_paket[4];
		                 ZamanAyarlimi = (gelen_paket[5]==1) ? TRUE : FALSE;		                 
		                 sayac = 0x01;
		            	 break;  
		            	 	           
		            default: 
		                 break;
        		}
        
        		if(sayac!=0)
        		{
	            	if(!mUSBGenTxIsBusy() )
	            	{
	                	USBGenWrite(gond_paket,sayac);
	                	sayac = 0;
	                	Komut = 0;
	               	}
        		}
        		   				   			
    		}	
    	}
    	StepmotorSur(Hiz,Yon,Motor);
	}
}
static void InitializeSystem(void) 
{
    mInitializeUSBDriver(); 
    //Port konfigürasyonlarý ve AD ayarlarý          
    LATB = 0x00; 
    TRISB = 0x00;
    TRISA = 0x03; 
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_0_TAD,ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS,0b1101); 
}


void USBTasks(void)
{
    USBCheckBusStatus();                    
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 

}


void ADC_Oku(void)			
{
	SetChanADC(ADC_CH0);	// AN0 
	Delay10TCYx( 20 );
	ConvertADC(); 			// A/D dönüþümünü baþlat 
	while( BusyADC() ); 	// Dönüþümün bitmesini bekle
	ADC_deger1 = ReadADC(); // Sonucu oku
	
	SetChanADC(ADC_CH1);	// AN1
	Delay10TCYx( 20 );
	ConvertADC(); // 
	while( BusyADC() ); 
	ADC_deger2 = ReadADC(); 
}
void StepmotorSur(byte nHiz, byte nYon , byte nMotor)
{
   	     	  
   	  if(ZamanAyarlimi)
      {	
     	 if(Adim == 0)
     	 {
     	 	motorPort=0;
     	 	return;	
         }
      	 else 	
        	Adim--; 
      } 
      	
      if(nYon != dur)      
      {
        		         	 	
        if(nMotor!= 0)
        {           
        	motorPort = (nMotor == 2)?(durumlar[adim_durum]&0xF0):(durumlar[adim_durum]&0x0F);
        }else
        {
	        motorPort = durumlar[adim_durum];
	    }
        Delay10KTCYx(nHiz); //~delay_ms(nHiz)
        
        if(nYon == sag)
          adim_durum = (adim_durum+1)&(sizeof(durumlar)-1);
        else if(nYon == sol)
          adim_durum = (adim_durum-1)&(sizeof(durumlar)-1);
       
      }
      motorPort=0;    
   
}
	