/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      ep0_alloc.h - EP0 Ram Yerleþimi              *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada uçnokta0 için setup paket'i ve    *
 *                  veri paketi yapýlarý tanýmlanmýþtýr.Bu yapý- *
 *                  lar Buffer Descriptor Table'lardan hemen     *
 *                  sonra tanýmalanýr.                           *
 *                  Bu alanlardaki yapýlar istek tiplerine göre  *
 *                  tanýmnaldýðýndan inceleme iþlemi daha kolay  *
 *                  yapýlýr.                                     *
 *****************************************************************/
#ifndef EP0_ALLOC_H
#define EP0_ALLOC_H

#define HID_IN_ENDPT_SIZE	    4
#define HID_OUT_ENDPT_SIZE	    4
#define ENDPOINT0_BUFFER_SIZE   0x08

//S E T U P   P A K E T 
typedef union _SETUP_PACKET
{
	struct
	{
		unsigned char Packet[ENDPOINT0_BUFFER_SIZE];
	};
	struct
	{
		unsigned char bmRequestType;
		unsigned char bRequest;
		unsigned int  wValue;
		unsigned int  wIndex;
		unsigned int  wLength;
	};
	struct
	{
		unsigned Recipient:5;          //
		unsigned RequestType:2;        //bmRequestType içeriði
		unsigned Direction:1;          //
		unsigned :8;
		unsigned char bFeature;        //Ýki özellik olabilir.DEVICE_REMOTE_WAKEUP, ENDPOINT_HALT
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned :8;
	};
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned char bDescriptorIndex;
		unsigned char bDescriptorType;
		unsigned int  wLanguageId;
		unsigned :8;
		unsigned :8;
	};//Konfigrasyon ve string künye istekleri için
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned char bDevAddrLow;
		unsigned char bDevAddrHigh;
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned :8;
	};//Set_Addres isteði için
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned char bConfigValue;
		unsigned char bConfigReserved;
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned :8;
	};//Konfigrasyon istekleri için (Set_Config..)
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned char bAlternateIdLow;
		unsigned char bAlternateIdHigh;
		unsigned char bInterfaceIdLow;
		unsigned char bInterfaceIdHigh;
		unsigned :8;
		unsigned :8;
	};//Interface istekleri için(Set_Interface..)
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned char bEndPointIdLow;
		unsigned char bEndPointIdHigh;
		unsigned :8;
		unsigned :8;
	};
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned :8;
		unsigned EndPointNum:4;
		unsigned :3;
		unsigned EndPointDir:1;
		unsigned :8;
		unsigned :8;
		unsigned :8;
	};
} SETUP_PACKET, *PSETUP_PACKET;	

//D A T A   P A C K E T 
typedef union DATA_PACKET
{
	struct
	{
		unsigned char DataPacket[ENDPOINT0_BUFFER_SIZE];
	};
	struct
	{
		unsigned char Data0;
		unsigned char Data1;
		unsigned char Data2;
		unsigned char Data3;
		unsigned char Data4;
		unsigned char Data5;
		unsigned char Data6;
		unsigned char Data7;
	};
} DATA_PACKET, *PDATA_PACKET;

#endif