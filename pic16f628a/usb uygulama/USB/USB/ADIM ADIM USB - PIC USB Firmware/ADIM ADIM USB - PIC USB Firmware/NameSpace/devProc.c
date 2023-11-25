/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      devProc.c - Cihaz g�revleri                  *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *																 *
 * A��klama:        Bu dosyada cihaz'�n as�l yapt��� g�revler'i  *
 *                  i�eren fonksiyonlar bulunmaktad�r.           *
 *****************************************************************/
//I N C L U D E 
#include "devproc.h"

//G L O B A L   D E � � � K E N L E R 
unsigned char PwmBuffer[HID_IN_ENDPT_SIZE];

/*****************************************************************
 * Fonksiyon Ismi:   UserInit()                                  *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon global bir fonksiyondur ve her *
 *                   devproc.c dosyas� taraf�ndan uygulanmak zo- *
 *                   rundad�r.Sistem ilk yap�land�r�l�rken bu    *
 *                   fonksiyonda �a�r�l�r.Bu y�zden kullan�c�    *
 *                   ba�lang�� i�lmeleri bu dosyada yap�lmal�d�r.*
 *****************************************************************/
void UserInit( void )
{
	mInitPwm();
}

/*****************************************************************
 * Fonksiyon Ismi:   UserProc()                                  *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyonda PC'den g�nderilen komutlara  *
 *                   g�re timer 2 a��l�r ve kapat�l�r.Ayn� za-   *
 *                   manda g�nderilen Pwm g�rev s�releri CCPR2L  *
 *                   ve CCP2CON<5:4> yazma�lar�na y�klenerek bir *
 *                   motorun h�z� kontrol edilir.                *
 *****************************************************************/
void UserProc( unsigned char Direction )
{
	if((bDeviceState < CONFIGURED)||(UCONbits.SUSPND == true)) return;
	if(Direction == INTERRUPT_OUT)
	{
		HidTransferReadService(PwmBuffer, HID_OUT_ENDPT_SIZE);
	}
	else if(Direction == INTERRUPT_IN)
	{
		if(!EndPoint1BdtIn.BDnSTAT.UOWN) HidTransferWriteService(PwmBuffer, HID_IN_ENDPT_SIZE);
	}
}


