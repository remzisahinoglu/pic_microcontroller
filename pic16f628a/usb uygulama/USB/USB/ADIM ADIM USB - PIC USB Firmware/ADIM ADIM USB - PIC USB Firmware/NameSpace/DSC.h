/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      DSC.h - Künye yapýlarý                       *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada windows tarafýndan istenen, cihaz *
 *                  hakkýndaki açýklayýcý bilgilerin bulunduðu   *
 *                  yapý bloklarý tanýmlanmýþtýr.                *
 *                  Bu yapýlardaki elemanlar gerekli bilgiler    *
 *                  ile doldurularak istekler'e gönderilir.      *
 *****************************************************************/
#ifndef DSC_H
#define DSC_H

//I N C L U D E S 
#include "usbdefs.h"

//C I H A Z   K U N Y E S I
typedef struct _DEVICE_DESCRIPTOR
{
	unsigned char bLength;                     //Künye uzunluðu
	unsigned char bDescriptorType;             //Künye tipi, sabit DEVICE(0x01)
	unsigned int  bcdUSB;                      //USB Spec. sürüm numarasý
	unsigned char bDeviceClass;                //Cihazýn desteklediði sýnýf kodu
	unsigned char bDeviceSubClass;             //Alt sýnýf kodu
	unsigned char bDeviceProtocol;             //Protokol kodu
	unsigned char bMaxPacketSize;              //Uçnokta0 max paket büyüklüðü
	unsigned int  idVendor;                    //Üretici ID
	unsigned int  idProduct;                   //Ürün ID
	unsigned int  bcdDevice;                   //Cihaz sürüm no
	unsigned char iManufacturer;               //Üretici string index'i
	unsigned char iProduct;                    //Ürün string index'i
	unsigned char iSerialNumber;               //Seri numarasý string index'i
	unsigned char bNumConfigurations;          //Desteklenen konfigrasyon adeti
} DEVICE_DESCRIPTOR, *PDEVICE_DESCRIPTOR; 

//C O N F I G R A S Y O N   K U N Y E S I 
typedef struct _CONFIGURATION_DESCRIPTOR
{
	unsigned char bLength;                     //Künye uzunluðu
	unsigned char bDescriptorType;             //Künye tipi, sabit CONFIGURATION(0x02)
	unsigned int  wTotalLength;                //Toplam konfigrasyon uzunluðu, arabirim ve uçnokta künyeleri dahil
	unsigned char bNumInterfaces;              //Desteklenen arabirim adedi
	unsigned char bConfigurationValue;         //Set_Config.. için yapýlandýrmayý tanýmlar
	unsigned char iConfiguration;              //Konfigrasyon string index'i
	unsigned char bmAttributes;                //Güç ve uzaktan uyandýrma özellikleri
	unsigned char MaxPower;                    //Bus güç deðerleri
} CONFIGURATION_DESCRIPTOR, *PCONFIGURATION_DESCRIPTOR;

//A R A B I R I M   K U N Y E S I
typedef struct _INTERFACE_DESCRIPTOR
{
	unsigned char bLength;                     //Künye uzunluðu
	unsigned char bDescriptorType;             //Künye tipi, sabit INTERFACE(0x04)
	unsigned char bInterfaceNumber;            //Arabirim'i temsil eden sayý
	unsigned char bAlternateSetting;           //Alternatif ayar deðeri
	unsigned char bNumEndpoints;               //Kontrol uçnoktasý hariç, desteklenen uçnokta sayýsý
	unsigned char bInterfaceClass;             //Cihaz sýnýfý kodu
	unsigned char bInterfaceSubClass;          //Alt sýnýfý kodu
	unsigned char bInterfaceProtocol;          //Protokol kodu
	unsigned char iInterface;                  //Arabirim string index'i
} INTERFACE_DESCRIPTOR, *PINTERFACE_DESCRIPTOR;

//U Ç N O K T A   K U N Y E S I
typedef struct _ENDPOINT_DESCRIPTOR
{
	unsigned char bLength;                     //Künye uzunluðu
	unsigned char bDescriptorType;             //Künye tipi, sabit ENDPOINT(0x05)
	unsigned char bEndPointAddress;            //Uçnokta yönü
	unsigned char bmAttributes;                //Desteklenen transfer tipi
	unsigned int  wMaxPacketSize;              //Desteklenen max paket boyutu
	unsigned char bInterval;                   //Yoklama süresi/max gecikme/max NAK
} ENDPOINT_DESCRIPTOR, *PENDPOINT_DESCRIPTOR;

//H I D   K U N Y E S I
//Ast künyeler için 
typedef struct _HID_DESCRIPTOR_HEADER
{
	unsigned char bDescriptorType;             //Ast künyenin tipi       
	unsigned int  wDescriptorLength;           //Ast künyenin uzunluðu
} HID_DESCRIPTOR_HEADER;

typedef struct _HID_DESCRIPTOR
{
	unsigned char bLength;                     //Künye uzunluðu
	unsigned char bDescriptorType;             //Künye tipi, sabit HID(0x21)
	unsigned int  bcdHID;                      //Cihazýn ve künyenin uyumlu olduðu HID spec. no
	unsigned char bCountryCode;                //Ülkeyi gösteren kod(Spec'de belirtilmiþtir)
	unsigned char bNumDescriptor;              //Ast künyelerin sayýsý
	HID_DESCRIPTOR_HEADER HidDscHeader[1];     //Ýlave ast künyeler
} HID_DESCRIPTOR, *PHID_DESCRIPTOR;

//C O N F I G U R A T I O N   T O P L A M   K U N Y E
typedef rom struct _CONFIG_1
{
	CONFIGURATION_DESCRIPTOR  ConfigDescriptor;
	INTERFACE_DESCRIPTOR	  InterfaceDescriptor;
	HID_DESCRIPTOR			  HidDescriptor;
	ENDPOINT_DESCRIPTOR		  EndPointDescriptor;
	ENDPOINT_DESCRIPTOR		  HidEndPointDescriptor;
} CONFIG_1, *PCONFIG_1; 

//E X T E R N S 
extern rom DEVICE_DESCRIPTOR  DeviceDescriptor;
extern CONFIG_1 ConfigurationDescriptor;
extern rom const unsigned char *rom USBConfigDescPtr[];
extern rom const unsigned char *rom USBStringDescPtr[];
extern rom struct { unsigned char Report[HID_REPORT_SIZE];} HidReport;  

#endif