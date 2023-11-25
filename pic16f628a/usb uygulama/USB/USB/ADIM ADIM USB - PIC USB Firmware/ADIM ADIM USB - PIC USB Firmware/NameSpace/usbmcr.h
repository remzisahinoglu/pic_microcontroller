/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbmcr.h - USB Macrolarý                     *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada USB haberleþmesinde kullanýlan    *
 *                  yardýmcý makrolar tanýmlanmýþtýr.            *
 *****************************************************************/
#ifndef USBMCR_H
#define USBMCR_H

//I N C L U D E S
#include <p18cxxx.h>
#include <delays.h>
#include "mem_alloc.h"
#include "pubfunc.h"
#include "DSC.h"

/*****************************************************************
 * Macro Ismi:     mInitUSB()                                    *
 *																 *
 * Parametreler:   -                                             *
 *																 *
 * Açýklama:       Bu makro USB modülü aktif etmeden önce UCFG   *
 *                 yazmacýný yapýlandýrýr.USB modül, Ping-Pong   *
 *                 Buffer disable, Full-Speed,Dahili dirençler   *
 *                 aktif ve dahili tranceiver aktif olmak üzere  *
 *                 kurulur.Cihaz baþlangýç durumuna alýnýr,aktif *
 *                 konfigrasyon ve cihaz özellikleri resetlenir. *
 *****************************************************************/
#define mInitUSB() {               \
	UCFG = PRM_UCFG;               \
	bDeviceState = DETACHED;       \
	UsbDeviceStatus.DevStat = 0;   \
	bActiveConfiguration = 0;      \
}

/*****************************************************************
 * Macro Ismi:     mInitEndPoint()                               *
 *																 *
 * Parametreler:   Ýþlem yapýlacak uçnokta buffer descriptor     *
 *																 *
 *                 status register'ý                             *
 * Açýklama:       Bu makro parametre olarak verilen uçnokta'nýn *
 *                 DTS bitinin durumunu tersler ve sonraki iþlem *
 *                 için kurar.                                   *
 *****************************************************************/
#define mInitEndPoint(endpoint) {                    \
	endpoint.BDnSTAT.BdsArea &= _DTSMASK;            \
	endpoint.BDnSTAT.DTS = !endpoint.BDnSTAT.DTS;    \
	endpoint.BDnSTAT.BdsArea |= _USIE|_DTSEN;        \
} 

/*****************************************************************
 * Macro Ismi:     mGetHidDscAdr()                               *
 *																 *
 * Parametreler:   Rom tip iþaretçi                              *
 *																 *
 * Açýklama:       Bu makro HID künyesinin adresini döndürür.    *
 *****************************************************************/
#define mGetHidDscAdr(hwnd) {                                               \
	if(bActiveConfiguration == 1)                                           \
	   hwnd = (rom unsigned char*)&ConfigurationDescriptor.HidDescriptor;   \
}	

/*****************************************************************
 * Macro Ismi:     mGetHidReportDscAdr()                         *
 *																 *   
 * Parametreler:   Rom tip iþaretçi                              *
 *																 *
 * Açýklama:       Bu makro HID Rapor künyesinin adresini döndü- *
 *                 rür.                                          *
 *****************************************************************/
#define mGetHidReportDscAdr(hwnd) {             \
	if(bActiveConfiguration == 1)               \
	   hwnd = (rom unsigned char*)&HidReport;   \
}

/*****************************************************************
 * Macro Ismi:     mWaitResumeSignal                             *
 *																 *
 * Parametreler:   Gecikme süresi								 *
 *																 *
 * Açýklama:       Bu makro RESUME sinyali gönderimi için gecik- *
 *                 me yapar.                                     *
 *****************************************************************/
#define mWaitResumeSignal(count) {              \
	do {                                        \
		count--;                                \
	} while(count);                             \
}

/*****************************************************************
 * Macro Ismi:     mDisableAllEndPoint()                         *
 *																 *
 * Parametreler:   -											 *
 *																 *
 * Açýklama:       Bu makro tüm uçnoktalarý devre dýþý býrakýr.  *
 *****************************************************************/
#define mDisableAllEndPoint() {                 \
	FlushBuffer((unsigned char*)&UEP1, 15);     \
}

/*****************************************************************
 * Macro Ismi:     mDisableAllEndPoint()                         *
 *																 *
 * Parametreler:   -											 *
 *																 *
 * Açýklama:       Bu makro tüm uçnoktalarý devre dýþý býrakýr.  *
 *****************************************************************/
#define mCheckAddressPending() {                                     \
	if(bDeviceState == ADR_PENDIG)                                   \
	{                                                                \
		UADDR = SetupPacket.bDevAddrLow;                             \
		if(UADDR > 0)                                                \
		   bDeviceState = ADDRESS;                                   \
		else                                                         \
		   bDeviceState = DEFAULT;                                   \
    }                                                                \
} 

/*****************************************************************
 * Macro Ismi:    mGetHidReportDescriptorSize()                  *
 *																 *
 * Parametreler:  Uzunluðu alýnacaðý deðiþken                    *
 * 																 *
 * Açýklama:      Bu makro HID Rapor künyesinin uzunluðunu dön-  *
 *                dürür.                                         *
 *****************************************************************/
#define mGetHidReportDescriptorSize(len) {                 \
	if(bActiveConfiguration == 1)                          \
	   len = sizeof(HidReport);                            \
}

/*****************************************************************
 * Macro Ismi:     LowLoad()                                     *
 *																 * 
 * Parametreler:   Veri yüklenecek deðiþken ve data              *
 *																 *
 * Açýklama:       Bu makro verilen deðiþkenin alt byte'ýna ve-  *
 *                 rilen deðeri yükler.                          *
 *****************************************************************/
#define LowLoad(var, data)      (var) |= (data)

/*****************************************************************
 * Macro Ismi:     HighLoad()                                    *
 *																 *
 * Parametreler:   Veri yüklenecek deðiþken ve data              *
 *																 *
 * Açýklama:       Bu makro verilen deðiþkenin üst byte'ýna ve-  *
 *                 rilen deðeri yükler.                          *
 *****************************************************************/
#define HighLoad(var, data)     (var) |= ((data)<<8) 

#define ToggleLed() {      \
	TEST_LED = 1;          \
	Delay10KTCYx(10);      \
	TEST_LED = 0;          \
	Delay10KTCYx(10);      \
}

/*****************************************************************
 * K O N T R O L   M A C R O L A R I 
 *****************************************************************/
#define isSof()                (UIRbits.SOFIF && UIEbits.SOFIE)
#define isError()              (UIRbits.UERRIF && UIEbits.UERRIE)
#define isStall()              (UIRbits.STALLIF && UIEbits.STALLIE)
#define isReset()              (UIRbits.URSTIF && UIEbits.URSTIE)
#define isActive()             (UIRbits.ACTVIF && UIEbits.ACTVIE)
#define isSuspend()            (UIRbits.IDLEIF && UIEbits.IDLEIE)
#define isComplate()           (UIRbits.TRNIF && UIEbits.TRNIE)

#define LSB(var)                (var) & 0xff
#define MSB(var)                ((var)>>8) & 0xff

#endif