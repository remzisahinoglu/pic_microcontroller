/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      devProc.c - Cihaz görevleri                  *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *																 *
 * Açýklama:        Bu dosyada cihaz'ýn asýl yaptýðý görevler'i  *
 *                  içeren fonksiyonlar bulunmaktadýr.           *
 *****************************************************************/
//I N C L U D E 
#include "devproc.h"

//G L O B A L   D E Ð Ý Þ K E N L E R 
unsigned char CmdBuffer[HID_IN_ENDPT_SIZE];

/*****************************************************************
 * Fonksiyon Ismi:   UserInit()                                  *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyon global bir fonksiyondur ve her *
 *                   devproc.c dosyasý tarafýndan uygulanmak zo- *
 *                   rundadýr.Sistem ilk yapýlandýrýlýrken bu    *
 *                   fonksiyonda çaðrýlýr.Bu yüzden kullanýcý    *
 *                   baþlangýç iþlmeleri bu dosyada yapýlmalýdýr.*
 *****************************************************************/
void UserInit( void )
{
	ROLE_PORT_TRIS = 0x00;
	ROLE_PORT 	   = 0x00;
}

/*****************************************************************
 * Fonksiyon Ismi:   UserProc()                                  *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * Dönen Deðer:      void										 *
 *																 *
 * Açýklama:         Bu fonksiyonda PC'den gönderilen komutlara  *
 *                   göre timer 2 açýlýr ve kapatýlýr.Ayný za-   *
 *                   manda gönderilen Pwm görev süreleri CCPR2L  *
 *                   ve CCP2CON<5:4> yazmaçlarýna yüklenerek bir *
 *                   motorun hýzý kontrol edilir.                *
 *****************************************************************/
void UserProc( unsigned char Direction )
{
	if((bDeviceState < CONFIGURED)||(UCONbits.SUSPND == true)) return;
	if(Direction == INTERRUPT_OUT)
	{
		HidTransferReadService(CmdBuffer, HID_OUT_ENDPT_SIZE);
		switch(CmdBuffer[DEV_INDEX])
		{
			case DEVICE_0:
				ROLE_PORT |= CmdBuffer[CMD_INDEX];
				break;
			case DEVICE_1:
				ROLE_PORT |= (CmdBuffer[CMD_INDEX] << 1);
				break;
			case DEVICE_2:
				ROLE_PORT |= (CmdBuffer[CMD_INDEX] << 2);
				break;
			case DEVICE_3:
				ROLE_PORT |= (CmdBuffer[CMD_INDEX] << 3);
				break;
		}
	}
	else if(Direction == INTERRUPT_IN)
	{
		if(!EndPoint1BdtIn.BDnSTAT.UOWN) HidTransferWriteService(CmdBuffer, HID_IN_ENDPT_SIZE);
	}
}


