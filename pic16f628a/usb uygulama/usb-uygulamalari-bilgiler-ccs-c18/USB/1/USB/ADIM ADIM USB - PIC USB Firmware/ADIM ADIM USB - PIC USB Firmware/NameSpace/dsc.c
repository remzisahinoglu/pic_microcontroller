/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      dsc.c - K�nyeler                             *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada Windows'un isteklerine kar��l�k   *
 *                  g�nderilen k�nye yap�lar� tan�mlanm��t�r.    *
 *****************************************************************/
//I N C L U D E S
#include "DSC.h"
#include "ep0_alloc.h"

//A Y G I T   K � N Y E S �
rom DEVICE_DESCRIPTOR  DeviceDescriptor = 
{
	sizeof(DEVICE_DESCRIPTOR),                           //K�nye Boyutu
	DEVICE,                                              //K�nye Tipi : Sabit DEVICE[0x01]
	0x0200,                                              //USB Spesifikasyon S�r�m No [2.0]
	0x00,                                                //Cihaz S�n�f Kodu
	0x00,                                                //Cihaz Alt S�n�f Kodu
	0x00,                                                //Protokol Kodu
	ENDPOINT0_BUFFER_SIZE,                               //U�nokta 0 Maximum Paket B�y�kl��� [8]
	0x1986,                                              //Vendor ID 
	0x3543,                                              //Product ID
	0x0001,                                              //Cihaz S�r�m No [1.0]
	0x01,                                                //�retici String K�nye Index'i
	0x02,                                                //�r�n String K�nye Index'i
	0x03,                                                //Seri Numaras� String K�nye Index'i
	0x01                                                 //Konfigrasyon Adedi
};

//K O N F I G U R A S Y O N   K � N Y E S �
CONFIG_1 ConfigurationDescriptor = 
{
	//Konfigrasyon k�nyesi
	sizeof(CONFIGURATION_DESCRIPTOR),                    //K�nye Boyutu
	CONFIGURATION,                                       //K�nye Tipi : Sabit CONFIGURATION[0x02]
	sizeof(CONFIG_1),                                    //Arabirim, U�nokta ve HID Dahil Toplam Uzunluk
	0x01,                                                //Arabirim Say�s�
	0x01,                                                //Konfigrasyon Index De�eri
	0x00,                                                //Konfigrasyon String K�nye Index'i
   _DEFAULT|_RWU,                                        //G�� ve Uzaktan uyand�rma ayarlar� [Remote WakeUp : ON]
    50,                                                  //Maximum Power [100 Miliamper] (Her de�erin yar�s� verilir.�rn:100 i�in 50)
   
   //Arabirim k�nyesi
	sizeof(INTERFACE_DESCRIPTOR),                        //K�nye Boyutu
	INTERFACE,                                           //K�nye Tipi : Sabit INTERFACE[0x04]
	0x00,                                                //Arabirimi belirleyen say�
	0x00,                                                //Alternatif Ayar de�eri
	0x02,                                                //Kontrol U�noktas� hari�(EP0) desteklenen u�nokta adedi
	HID_INTERFACE_CODE,                                  //Arabirimin uyumlu oldu�u s�n�f kodu [HID Uyumlu]
	0x00,                                                //Alt S�n�f Kodu
	0x00,                                                //Protokol Kodu
	0x00,                                                //Arabirim String K�nye Index'i
	
	//HID K�nyesi
	sizeof(HID_DESCRIPTOR),                              //K�nye Boyutu
	HID_DESC,                                            //K�nye Tipi : Sabit HID[0x21]
	0x0101,                                              //HID S�r�m No [1.0]
	0x00,                                                //�lke Kodu
	HID_DESC_NUM,                                        //Desteklenen Alt S�n�f K�nye Adedi
	REPORT_DESC,                                         //K�nye S�n�f�'n�n Tipi
	sizeof(HidReport),                                   //K�nye Toplam Uzunlu�u
	
	//U�Nokta k�nyesi
	sizeof(ENDPOINT_DESCRIPTOR),                         //K�nye Boyutu
	ENDPOINT,                                            //K�nye Tipi : Sabit ENDPOINT[0x05]
   _EP01_IN,                                             //U�nokta Say�s� ve Y�n�
    INTERRUPT,                                           //Transfer Tipi [Kesme Transfer]
    HID_IN_ENDPT_SIZE,                                   //Bu u�noktan�n destekledi�i Max Paket boyutu
    0xA0,                                                //Yoklama s�resi [10ms]
    
    sizeof(ENDPOINT_DESCRIPTOR),
    ENDPOINT,
   _EP01_OUT,
    INTERRUPT,
    HID_OUT_ENDPT_SIZE,
    0xA0
};

//S T R I N G   K U N Y E L E R I 

//K�nye 1
//Dil bilgileri
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[1]; } StringDescriptor_0 =
{
	sizeof(StringDescriptor_0),
	STRING,
	0x0409
};

//K�nye 2 [Manufacturer String]
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[9]; } StringDescriptor_1 =
{
	sizeof(StringDescriptor_1),
	STRING,
	'J','a','B','B','a','S','o','f','t'
};

//K�nye 3 [Product String]
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[24]; } StringDescriptor_2 =
{
	sizeof(StringDescriptor_2),
	STRING,
	'J','a','B','B','a','S','o','f','t',' ','U','S','B',' ','U','y','g','u','l','a','m','a','s','i'
};

//K�nye 3 [Serial Number String]
rom struct { unsigned char bLength; unsigned char bDescriptorType; unsigned int String[14]; } StringDescriptor_3 = 
{
	sizeof(StringDescriptor_3),
	STRING,
	'J','1','B','0','B','1','8','A','3','D','3','5','4','3'
};

//H I D   R A P O R   K � N Y E S � 
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

//�stenen index de�erlerine g�re istenen k�nyelerin g�nderilebilmesi i�in
//k�nye ba�lang�� adresleri index'ine g�re bir dizide tutuluyor.
rom const unsigned char *rom USBConfigDescPtr[] = {&ConfigurationDescriptor};
rom const unsigned char *rom USBStringDescPtr[] = {&StringDescriptor_0, 
	                                               &StringDescriptor_1, 
	                                               &StringDescriptor_2,
	                                               &StringDescriptor_3
	                                               };