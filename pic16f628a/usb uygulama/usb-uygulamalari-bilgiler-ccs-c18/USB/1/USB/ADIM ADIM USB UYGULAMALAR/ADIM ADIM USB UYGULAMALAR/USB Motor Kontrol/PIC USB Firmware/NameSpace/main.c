/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      main.c                                       *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************/
//I N C L U D E S 
#include <p18cxxx.h>
#include "usbmcr.h"
#include "pubfunc.h"

//F O N K S I Y O N   P R O T O T I P 
void USBProcess( void );
void InitMCUSystem( void );

/*****************************************************************
 * Fonksiyon Ismi:   InitMCUSystem()                             *
 *                   											 *
 * Parametreler:     void                                        *
 *																 *
 * Dönen Deðer:      void                                        *
 *																 *
 * Açýklama:         Bu fonksyion sistem baþlangýç ayarlarýný ya-*
 *                   par ve USB'yi yapýlandýrýr.                 *
 *****************************************************************/
void InitMCUSystem( void )
{
	TEST_LED_TRIS		   = CLEAR;
	USB_ATTACH_REMOVE_TRIS = SET;
	USB_ATTACH_REMOVE_PIN  = CLEAR;
	
	mInitUSB();                             //USB Konfigrasyon ayarý
	UserInit();                             //Kullanýcý ayarlarý
}

/*****************************************************************
 * Fonksiyon Ismi:   USBProcess()                                *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon cihaz'ýn BUS'a takýlý olup ol- *
 *                   madýðýný ve USB olaylarýný kontrol eder.    *
 *****************************************************************/
void USBProcess( void )
{
	USBCheckBus();
	if(!UCFGbits.UTEYE) 
		USBService();
}

//////////////////////////////////////////////////////////////////
//                   A N A   P R O G R A M                      // 
//////////////////////////////////////////////////////////////////
void main( void )
{
	InitMCUSystem();
	while(true) USBProcess(); 
}