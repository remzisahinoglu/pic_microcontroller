/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      DSC.h - K�nye yap�lar�                       *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada windows taraf�ndan istenen, cihaz *
 *                  hakk�ndaki a��klay�c� bilgilerin bulundu�u   *
 *                  yap� bloklar� tan�mlanm��t�r.                *
 *                  Bu yap�lardaki elemanlar gerekli bilgiler    *
 *                  ile doldurularak istekler'e g�nderilir.      *
 *****************************************************************/
#ifndef DSC_H
#define DSC_H

//I N C L U D E S 
#include "usbdefs.h"

//C I H A Z   K U N Y E S I
typedef struct _DEVICE_DESCRIPTOR
{
	unsigned char bLength;                     //K�nye uzunlu�u
	unsigned char bDescriptorType;             //K�nye tipi, sabit DEVICE(0x01)
	unsigned int  bcdUSB;                      //USB Spec. s�r�m numaras�
	unsigned char bDeviceClass;                //Cihaz�n destekledi�i s�n�f kodu
	unsigned char bDeviceSubClass;             //Alt s�n�f kodu
	unsigned char bDeviceProtocol;             //Protokol kodu
	unsigned char bMaxPacketSize;              //U�nokta0 max paket b�y�kl���
	unsigned int  idVendor;                    //�retici ID
	unsigned int  idProduct;                   //�r�n ID
	unsigned int  bcdDevice;                   //Cihaz s�r�m no
	unsigned char iManufacturer;               //�retici string index'i
	unsigned char iProduct;                    //�r�n string index'i
	unsigned char iSerialNumber;               //Seri numaras� string index'i
	unsigned char bNumConfigurations;          //Desteklenen konfigrasyon adeti
} DEVICE_DESCRIPTOR, *PDEVICE_DESCRIPTOR; 

//C O N F I G R A S Y O N   K U N Y E S I 
typedef struct _CONFIGURATION_DESCRIPTOR
{
	unsigned char bLength;                     //K�nye uzunlu�u
	unsigned char bDescriptorType;             //K�nye tipi, sabit CONFIGURATION(0x02)
	unsigned int  wTotalLength;                //Toplam konfigrasyon uzunlu�u, arabirim ve u�nokta k�nyeleri dahil
	unsigned char bNumInterfaces;              //Desteklenen arabirim adedi
	unsigned char bConfigurationValue;         //Set_Config.. i�in yap�land�rmay� tan�mlar
	unsigned char iConfiguration;              //Konfigrasyon string index'i
	unsigned char bmAttributes;                //G�� ve uzaktan uyand�rma �zellikleri
	unsigned char MaxPower;                    //Bus g�� de�erleri
} CONFIGURATION_DESCRIPTOR, *PCONFIGURATION_DESCRIPTOR;

//A R A B I R I M   K U N Y E S I
typedef struct _INTERFACE_DESCRIPTOR
{
	unsigned char bLength;                     //K�nye uzunlu�u
	unsigned char bDescriptorType;             //K�nye tipi, sabit INTERFACE(0x04)
	unsigned char bInterfaceNumber;            //Arabirim'i temsil eden say�
	unsigned char bAlternateSetting;           //Alternatif ayar de�eri
	unsigned char bNumEndpoints;               //Kontrol u�noktas� hari�, desteklenen u�nokta say�s�
	unsigned char bInterfaceClass;             //Cihaz s�n�f� kodu
	unsigned char bInterfaceSubClass;          //Alt s�n�f� kodu
	unsigned char bInterfaceProtocol;          //Protokol kodu
	unsigned char iInterface;                  //Arabirim string index'i
} INTERFACE_DESCRIPTOR, *PINTERFACE_DESCRIPTOR;

//U � N O K T A   K U N Y E S I
typedef struct _ENDPOINT_DESCRIPTOR
{
	unsigned char bLength;                     //K�nye uzunlu�u
	unsigned char bDescriptorType;             //K�nye tipi, sabit ENDPOINT(0x05)
	unsigned char bEndPointAddress;            //U�nokta y�n�
	unsigned char bmAttributes;                //Desteklenen transfer tipi
	unsigned int  wMaxPacketSize;              //Desteklenen max paket boyutu
	unsigned char bInterval;                   //Yoklama s�resi/max gecikme/max NAK
} ENDPOINT_DESCRIPTOR, *PENDPOINT_DESCRIPTOR;

//H I D   K U N Y E S I
//Ast k�nyeler i�in 
typedef struct _HID_DESCRIPTOR_HEADER
{
	unsigned char bDescriptorType;             //Ast k�nyenin tipi       
	unsigned int  wDescriptorLength;           //Ast k�nyenin uzunlu�u
} HID_DESCRIPTOR_HEADER;

typedef struct _HID_DESCRIPTOR
{
	unsigned char bLength;                     //K�nye uzunlu�u
	unsigned char bDescriptorType;             //K�nye tipi, sabit HID(0x21)
	unsigned int  bcdHID;                      //Cihaz�n ve k�nyenin uyumlu oldu�u HID spec. no
	unsigned char bCountryCode;                //�lkeyi g�steren kod(Spec'de belirtilmi�tir)
	unsigned char bNumDescriptor;              //Ast k�nyelerin say�s�
	HID_DESCRIPTOR_HEADER HidDscHeader[1];     //�lave ast k�nyeler
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