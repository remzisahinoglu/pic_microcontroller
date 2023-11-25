/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbmcr.h - USB Macrolar�                     *
 *																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada USB haberle�mesinde kullan�lan    *
 *                  yard�mc� makrolar tan�mlanm��t�r.            *
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
 * A��klama:       Bu makro USB mod�l� aktif etmeden �nce UCFG   *
 *                 yazmac�n� yap�land�r�r.USB mod�l, Ping-Pong   *
 *                 Buffer disable, Full-Speed,Dahili diren�ler   *
 *                 aktif ve dahili tranceiver aktif olmak �zere  *
 *                 kurulur.Cihaz ba�lang�� durumuna al�n�r,aktif *
 *                 konfigrasyon ve cihaz �zellikleri resetlenir. *
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
 * Parametreler:   ��lem yap�lacak u�nokta buffer descriptor     *
 *																 *
 *                 status register'�                             *
 * A��klama:       Bu makro parametre olarak verilen u�nokta'n�n *
 *                 DTS bitinin durumunu tersler ve sonraki i�lem *
 *                 i�in kurar.                                   *
 *****************************************************************/
#define mInitEndPoint(endpoint) {                    \
	endpoint.BDnSTAT.BdsArea &= _DTSMASK;            \
	endpoint.BDnSTAT.DTS = !endpoint.BDnSTAT.DTS;    \
	endpoint.BDnSTAT.BdsArea |= _USIE|_DTSEN;        \
} 

/*****************************************************************
 * Macro Ismi:     mGetHidDscAdr()                               *
 *																 *
 * Parametreler:   Rom tip i�aret�i                              *
 *																 *
 * A��klama:       Bu makro HID k�nyesinin adresini d�nd�r�r.    *
 *****************************************************************/
#define mGetHidDscAdr(hwnd) {                                               \
	if(bActiveConfiguration == 1)                                           \
	   hwnd = (rom unsigned char*)&ConfigurationDescriptor.HidDescriptor;   \
}	

/*****************************************************************
 * Macro Ismi:     mGetHidReportDscAdr()                         *
 *																 *   
 * Parametreler:   Rom tip i�aret�i                              *
 *																 *
 * A��klama:       Bu makro HID Rapor k�nyesinin adresini d�nd�- *
 *                 r�r.                                          *
 *****************************************************************/
#define mGetHidReportDscAdr(hwnd) {             \
	if(bActiveConfiguration == 1)               \
	   hwnd = (rom unsigned char*)&HidReport;   \
}

/*****************************************************************
 * Macro Ismi:     mWaitResumeSignal                             *
 *																 *
 * Parametreler:   Gecikme s�resi								 *
 *																 *
 * A��klama:       Bu makro RESUME sinyali g�nderimi i�in gecik- *
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
 * A��klama:       Bu makro t�m u�noktalar� devre d��� b�rak�r.  *
 *****************************************************************/
#define mDisableAllEndPoint() {                 \
	FlushBuffer((unsigned char*)&UEP1, 15);     \
}

/*****************************************************************
 * Macro Ismi:     mDisableAllEndPoint()                         *
 *																 *
 * Parametreler:   -											 *
 *																 *
 * A��klama:       Bu makro t�m u�noktalar� devre d��� b�rak�r.  *
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
 * Parametreler:  Uzunlu�u al�naca�� de�i�ken                    *
 * 																 *
 * A��klama:      Bu makro HID Rapor k�nyesinin uzunlu�unu d�n-  *
 *                d�r�r.                                         *
 *****************************************************************/
#define mGetHidReportDescriptorSize(len) {                 \
	if(bActiveConfiguration == 1)                          \
	   len = sizeof(HidReport);                            \
}

/*****************************************************************
 * Macro Ismi:     LowLoad()                                     *
 *																 * 
 * Parametreler:   Veri y�klenecek de�i�ken ve data              *
 *																 *
 * A��klama:       Bu makro verilen de�i�kenin alt byte'�na ve-  *
 *                 rilen de�eri y�kler.                          *
 *****************************************************************/
#define LowLoad(var, data)      (var) |= (data)

/*****************************************************************
 * Macro Ismi:     HighLoad()                                    *
 *																 *
 * Parametreler:   Veri y�klenecek de�i�ken ve data              *
 *																 *
 * A��klama:       Bu makro verilen de�i�kenin �st byte'�na ve-  *
 *                 rilen de�eri y�kler.                          *
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