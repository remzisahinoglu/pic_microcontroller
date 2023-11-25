/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      dsc.c - Künyeler                             *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada Windows'un isteklerine karþýlýk   *
 *                  gönderilen künye yapýlarý tanýmlanmýþtýr.    *
 *****************************************************************/
//I N C L U D E S
#include "DSC.h"
#include "ep0_alloc.h"

//A Y G I T   K Ü N Y E S Ý
rom DEVICE_DESCRIPTOR  DeviceDescriptor = 
{
	sizeof(DEVICE_DESCRIPTOR),                           //Künye Boyutu
	DEVICE,                                              //Künye Tipi : Sabit DEVICE[0x01]
	0x0200,                                              //USB Spesifikasyon Sürüm No [2.0]
	0x00,                                                //Cihaz Sýnýf Kodu
	0x00,                                                //Cihaz Alt Sýnýf Kodu
	0x00,                                                //Protokol Kodu
	ENDPOINT0_BUFFER_SIZE,                               //Uçnokta 0 Maximum Paket Büyüklüðü [8]
	0x1986,                                              //Vendor ID 
	0x3543,                                              //Product ID
	0x0001,                                              //Cihaz Sürüm No [1.0]
	0x01,                                                //Üretici String Künye Index'i
	0x02,                                                //Ürün String Künye Index'i
	0x03,                                                //Seri Numarasý String Künye Index'i
	0x01                                                 //Konfigrasyon Adedi
};

//K O N F I G U R A S Y O N   K Ü N Y E S Ý
CONFIG_1 ConfigurationDescriptor = 
{
	//Konfigrasyon künyesi
	sizeof(CONFIGURATION_DESCRIPTOR),                    //Künye Boyutu
	CONFIGURATION,                                       //Künye Tipi : Sabit CONFIGURATION[0x02]
	sizeof(CONFIG_1),                                    //Arabirim, Uçnokta ve HID Dahil Toplam Uzunluk
	0x01,                                                //Arabirim Sayýsý
	0x01,                                                //Konfigrasyon Index Deðeri
	0x00,                                                //Konfigrasyon String Künye Index'i
   _DEFAULT|_RWU,                                        //Güç ve Uzaktan uyandýrma ayarlarý [Remote WakeUp : ON]
    50,                                                  //Maximum Power [100 Miliamper] (Her deðerin yarýsý verilir.Örn:100 için 50)
   
   //Arabirim künyesi
	sizeof(INTERFACE_DESCRIPTOR),                        //Künye Boyutu
	INTERFACE,                                           //Künye Tipi : Sabit INTERFACE[0x04]
	0x00,                                                //Arabirimi belirleyen sayý
	0x00,                                                //Alternatif Ayar deðeri
	0x02,                                                //Kontrol Uçnoktasý hariç(EP0) desteklenen uçnokta adedi
	HID_INTERFACE_CODE,                                  //Arabirimin uyumlu olduðu sýnýf kodu [HID Uyumlu]
	0x00,                                                //Alt Sýnýf Kodu
	0x00,                                                //Protokol Kodu
	0x00,                                                //Arabirim String Künye Index'i
	
	//HID Künyesi
	sizeof(HID_DESCRIPTOR),                              //Künye Boyutu
	HID_DESC,                                            //Künye Tipi : Sabit HID[0x21]
	0x0101,                                              //HID Sürüm No [1.0]
	0x00,                                                //Ülke Kodu
	HID_DESC_NUM,                                        //Desteklenen Alt Sýnýf Künye Adedi
	REPORT_DESC,                                         //Künye Sýnýfý'nýn Tipi
	sizeof(HidReport),                                   //Künye Toplam Uzunluðu
	
	//UçNokta künyesi
	sizeof(ENDPOINT_DESCRIPTOR),                         //Künye Boyutu
	ENDPOINT,                                            //Künye Tipi : Sabit ENDPOINT[0x05]
   _EP01_IN,                                             //Uçnokta Sayýsý ve Yönü
    INTERRUPT,                                           //Transfer Tipi [Kesme Transfer]
    HID_IN_ENDPT_SIZE,                                   //Bu uçnoktanýn desteklediði Max Paket boyutu
    0xA0,                                                //Yoklama süresi [10ms]
    
    sizeof(ENDPOINT_DESCRIPTOR),
    ENDPOINT,
   _EP01_OUT,
    INTERRUPT,
    HID_OUT_ENDPT_SIZE,
    0xA0
};

//S T R I N G   K U N Y E L E R I 

//Künye 1
//Dil bilgileri
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[1]; } StringDescriptor_0 =
{
	sizeof(StringDescriptor_0),
	STRING,
	0x0409
};

//Künye 2 [Manufacturer String]
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[9]; } StringDescriptor_1 =
{
	sizeof(StringDescriptor_1),
	STRING,
	'J','a','B','B','a','S','o','f','t'
};

//Künye 3 [Product String]
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[24]; } StringDescriptor_2 =
{
	sizeof(StringDescriptor_2),
	STRING,
	'J','a','B','B','a','S','o','f','t',' ','U','S','B',' ','U','y','g','u','l','a','m','a','s','i'
};

//Künye 3 [Serial Number String]
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[14]; } StringDescriptor_3 = 
{
	sizeof(StringDescriptor_3),
	STRING,
	'J','1','B','0','B','1','8','A','3','D','3','5','4','3'
};

//H I D   R A P O R   K Ü N Y E S Ý 
rom struct { unsigned char Report[HID_REPORT_SIZE];} HidReport =
{
	0x05, 0x01,
	0x09, 0x05,
	0xA1, 0x00,
	//Input Rapor
	0x09, 0x30,
	0x09, 0x31,
	0x15, 0x00,
	0x26, 0xFF, 0xFF,
	0x75, 0x10,
	0x95, 0x02,
	0x81, 0x02,
	//Output Rapor
	0x09, 0x30,
	0x09, 0x31,
	0x15, 0x00,
	0x26, 0xFF, 0xFF,
	0x75, 0x10,
	0x95, 0x02,
	0x91, 0x02,
	
	0xC0
};

//Ýstenen index deðerlerine göre istenen künyelerin gönderilebilmesi için
//künye baþlangýç adresleri index'ine göre bir dizide tutuluyor.
rom const unsigned char *rom USBConfigDescPtr[] = {&ConfigurationDescriptor};
rom const unsigned char *rom USBStringDescPtr[] = {&StringDescriptor_0, 
	                                               &StringDescriptor_1, 
	                                               &StringDescriptor_2,
	                                               &StringDescriptor_3
	                                               };