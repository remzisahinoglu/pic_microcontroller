/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      mem_alloc.h - USB Ram yerle�imi              *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada USB ram yerle�iminde kullan�lacak *
 *                  yap�lar tan�mlanm��t�r.mem_alloc.c dosyas�n- *
 *                  da bu yap�lar kullan�larak, usb i�in ayr�lan *
 *                  ram b�lgesi 0x400 adresinden itibaren �ekil- *
 *                  lendirilmi�tir.                              *
 *****************************************************************/
#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

//I N C L U D E S
#include "ep0_alloc.h"
#include "usbdefs.h"

//Cihaz �zellikleri.Sadece uzaktan uyand�rma tan�ml�d�r.
typedef union _DEVICE_STATUS
{
	unsigned char DevStat;
	struct 
	{ 
		unsigned RemoteWakeup:1;
		unsigned HandleType:1;
	};
} DEVICE_STATUS, *PDEVICE_STATUS;
	
/*
 * Bu alan ile ilgili tan�malamalar ve a��klamalar �ekilleriyle
 * birlikte 18f4550 DataSheet'inin 173. sayfas�nda "Buffer Descriptors and
 * the Buffer Descriptor Table" b�l�m�nde yer almaktad�r.
 */

//B U F F E R   D E S C R I P T O R   S T A T U S 
typedef union _BUFF_DESC_STAT
{
	unsigned char BdsArea;
	struct
	{
        unsigned BC8:1;                    //Byte count 8 (1024 byte veri i�in)
        unsigned BC9:1;                    //Byte count 9
        unsigned BSTALL:1;                 //Buffer stall enable bit
        unsigned DTSEN:1;                  //Veri de�i�im senkronizasyonu aktif bit
        unsigned INCDIS:1;                 //Adres artt�rma bit'i
        unsigned KEN:1;                    //Keep Enable bit
        unsigned DTS:1;                    //Veri de�i�im senkronizasyon bit
        unsigned UOWN:1;                   //USB Own bit
	};//Buffer Descriptor Status Register CPU Mod
	struct
	{
		unsigned :2;
		unsigned PID:4;
		unsigned :2;	
	};
} BUFF_DESC_STAT, *PBUFF_DESC_STAT;

//B U F F E R   D E S C R I P T O R   T A B L E 
typedef union _BUFF_DESC_TAB
{
	struct
	{
		BUFF_DESC_STAT BDnSTAT;            //Buffer descriptor status register
		unsigned char  BDnCNT;             //Buffer descriptor byte count register
		unsigned char  BDnADRL;            //Buffer adres low
		unsigned char  BDnADRH;            //Buffer adres high
	};
	struct
	{
		unsigned :8;
		unsigned :8;
		unsigned char *ADDR;
	};
} BUFF_DESC_TAB, *PBUFF_DESC_TAB;

//H A N D L E 
typedef union _HANDLE
{
	unsigned char *bHandle;
	unsigned int  *wHandle;

	rom unsigned char *brHandle;
	rom unsigned int  *wrHandle;
} HANDLE;

//************************************************************************
// E X T E R N S 
//************************************************************************
extern unsigned char bTransferState;
extern unsigned char bHidReportLength;
extern unsigned char bDeviceState;
extern unsigned char bActiveConfiguration;
extern unsigned char bAlternateInterface[MAX_INTERFACE_NUM];
extern unsigned char bSessionOwner;
extern unsigned int  iDataCount;
extern HANDLE hSource;
extern HANDLE hDestination;
extern DEVICE_STATUS UsbDeviceStatus;

extern volatile far BUFF_DESC_TAB   EndPoint0BdtOut;
extern volatile far BUFF_DESC_TAB	EndPoint0BdtIn;
extern volatile far BUFF_DESC_TAB	EndPoint1BdtOut;
extern volatile far BUFF_DESC_TAB	EndPoint1BdtIn;

extern volatile far SETUP_PACKET    SetupPacket;
extern volatile far DATA_PACKET		CntrlDataPacket;

extern volatile far unsigned char   HidReportOut[HID_OUT_ENDPT_SIZE];
extern volatile far unsigned char   HidReportIn[HID_IN_ENDPT_SIZE];
extern volatile far unsigned char   HidFeatureReport[ENDPOINT0_BUFFER_SIZE];

#endif