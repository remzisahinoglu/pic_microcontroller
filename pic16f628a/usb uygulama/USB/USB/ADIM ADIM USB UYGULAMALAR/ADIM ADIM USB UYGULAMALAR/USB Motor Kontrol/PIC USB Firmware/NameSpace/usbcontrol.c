/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      usbcontrol.c - Kontrol transferleri          *
 *															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * A��klama:        Bu dosyada kontrol transferleri i�in gerekli *
 *                  fonksiyonlar yer almaktad�r.                 *
 *****************************************************************/
//I N C L U D E S 
#include <p18cxxx.h>
#include "mem_alloc.h"
#include "usbdefs.h"
#include "pubfunc.h"
#include "usbmcr.h"

//� Z E L   F O N K S � Y O N   P R O T O T � P 
void USBControlInHandler( void );
void USBControlOutHandler( void );
void USBControlSetupHandler( void );

/*****************************************************************
 * Fonksiyon Ismi:   USBControlInHandler()                       *
 *																 *
 * Parametreler:     void										 *
 *     															 *
 * D�nen De�er:      void										 *
 * 																 *
 * A��klama:         Bu fonksiyon control i�lemi IN ise i�leti-  *
 *                   lir ve SETUP paketi al�nd�ktan sonra ��z�-  *
 *                   len iste�in gerektirdi�i veriler tampona    *
 *                   yerle�tirildikten sonra bu fonksiyon bu tam-*
 *                   pondaki verileri PC'ye g�nderir.SETUP pake- *
 *                   ti bir Set_Address iste�i i�eriyorsa cihaz  *
 *                   ADDRESS_PENDING durumuna al�n�r.Bir sonraki *
 *                   IN i�leminde ise cihaz�n adresli olup olma- *
 *                   d��� kontrol edilir.De�ilse wValue alan�    *
 *                   okunarak cihaz adresli duruma getirilir.    *
 *****************************************************************/
void USBControlInHandler( void )
{
	mCheckAddressPending();
	if(bTransferState == CTRL_SEND)
	{
		ControlTransferWriteService();
		if(!EndPoint0BdtIn.BDnSTAT.DTS)
		   EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		else
		   EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;
	}
	else
	   USBInitNextSetupTrf();
}

/*****************************************************************
 * Fonksiyon Ismi:   USBControlOutHandler()                      *
 *																 *
 * Parametreler:     void										 *
 *															     *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon control transfer i�lemi OUT    *
 *                   ise i�letilir ve SIE taraf�ndan doldurulan  *
 *                   CntrlDataPacket alan�n� okumak �zere ilgi-  *
 *                   li fonksiyonu �a��r�r.Ard�n senkronizasyon  *
 *                   bitinin durumunu de�i�tirir.                *
 *****************************************************************/
void USBControlOutHandler( void )
{
	if(bTransferState == CTRL_REC)
	{
		ControlTransferReadService();
		if(!EndPoint0BdtOut.BDnSTAT.DTS)
		   EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		else
		   EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;
	}
	else
	   USBInitNextSetupTrf();
}

/*****************************************************************
 * Fonksiyon Ismi:   USBControlSetupHandler()                    *
 *																 *
 * Parametreler:     void                                        *
 *															 	 *
 * D�nen De�er:      void										 *
 *                   											 *
 * A��klama:         Bu fonksiyon g�nderilen setup paketini in-  *
 *                   celeyip gerekli fonksiyonlar� �a��racak o-  *
 *                   lan fonksiyonlar� �a��r�r.                  *
 *                   Transfer i�lemi sahibi ba�lang��ta SESSION  *
 *                   NULL olarak ayarlan�r.�lgili fonksiyonlar   *
 *                   �a�r�ld�ktan sonra e�er istek standart is-  *
 *                   teklerden biri ise transfer i�lemi sahibi   *
 *                   SESSION_CNTRL olarak atan�r.E�er standart   *
 *                   isteklerden biri de�ilse transfer sahibi    *
 *                   SESSION_NULL olarak kalacak ve bu sefer �-  *
 *                   zel s�n�f istekleri(HID) kontrol edilecek-  *
 *                   tir.Son olarak i�lemleri tamamlayacak fonk- *
 *                   siyon �a�r�l�r.                             * 
 *****************************************************************/
void USBControlSetupHandler( void )
{
	bTransferState = WAIT;
	bSessionOwner  = SESSION_NULL;
	iDataCount     = CLEAR;
	
	ToggleLed();     //Setup paketleri geldi�i zaman led'i toggle yap
	
	CheckStandartRequest();
	if(bSessionOwner == SESSION_NULL) CheckHidRequest();
	ControlTransferComplateService();
}

/*****************************************************************
 * Fonksiyon Ismi:   EndPointService()                           *
 *																 *
 * Parametreler:     void                                        *
 *																 *
 * D�nen De�er:      void										 *
 *        														 *
 * A��klama:         Bu fonksiyon kontrol transfer tipini tes-   *
 *                   pit eder ve ilgili fonksiyonlar� �a��r�r.   *
 *                   Sadece u�nokta 0'a gelen istekler de�erlen- *
 *                   dirilir.Bu istekler SETUP, OUT, ve IN'dir.  *
 *                   Cihaz kurulu durumdaysa U�nokta1'e gelen    *
 *                   paketleride de�erlendirir.Bu paketler sade- *
 *                   ce IN veya OUT olabilir.                    *
 *****************************************************************/
void EndPointService( void )
{
	//Kontrol Transfer ��lemleri : OUT
	if(USTAT == EP0_OUT)
	{
		if(EndPoint0BdtOut.BDnSTAT.PID == SETUP_TOKEN) 
		   USBControlSetupHandler(); 
		else 
		   USBControlOutHandler();
	} //IN
	else if(USTAT == EP0_IN) USBControlInHandler();
	
	//Kesme transfer i�lemleri
	if(USTAT == EP1_OUT) 
	{ 
		UserProc(INTERRUPT_OUT); 
		UCONbits.PKTDIS = false; 
	}
	if(USTAT == EP1_IN)  
	{ 
		UserProc(INTERRUPT_IN);  
		UCONbits.PKTDIS = false;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ControlTransferWriteService()               *
 *																 *
 * Parametreler:     void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon IN paketi al�nd�ktan sonra bel-*
 *                   lek b�lgesine yerle�tirilen (RAM-ROM) bil-  *
 *                   gileri g�nderir.                            *
 *****************************************************************/
void ControlTransferWriteService( void )
{
	//Kontrol Read Transfer
	//PC veri basama��nda al�m yapar.
	
	unsigned int iSendCount;
	
	//G�nderilecek veri adedi u�nokta0'dan b�y�kse u�nokta0 b�y�kl���
	//kadar veri g�nderilir.Aksi halde t�m veriler g�nderilir.
	if(iDataCount < ENDPOINT0_BUFFER_SIZE)
	   iSendCount = iDataCount;
	else
	   iSendCount = ENDPOINT0_BUFFER_SIZE;
	   
	//G�nderilecek verilerin adedi buffer descriptor table'a y�kleniyor.
	EndPoint0BdtIn.BDnSTAT.BC9 = false;                   //�st bitleri ilk olarak s�f�rla
	EndPoint0BdtIn.BDnSTAT.BC8 = false;
	EndPoint0BdtIn.BDnSTAT.BdsArea |= MSB(iSendCount);    //�imdi BC9 ve BC8'e �st bitleri y�kle
	EndPoint0BdtIn.BDnCNT = LSB(iSendCount);              //Son olarak alt byte'� CNT'ye y�kle
	
	//Geriye g�nderilecek veri adedini iDataCount'a y�kle
	iDataCount -= iSendCount;
	
	//Verilerin yaz�laca�� hedef'in handle'�n� al
	hDestination.bHandle = (unsigned char*)&CntrlDataPacket;
	
	//K�nye bilgileri gibi baz� bilgiler rom bellekte tan�ml� oldu�u i�in
	//transfer t�r� ROM_TYPE se�ilir.Di�er transfer'lerde RAM_TYPE kullan�l�r.
	if(UsbDeviceStatus.HandleType == ROM_TYPE)
	{
		while(iSendCount) {
		    *hDestination.bHandle = *hSource.brHandle;
			hSource.brHandle++;
			hDestination.bHandle++;
			iSendCount--;
		}
	}// RAM_TYPE
	else
	{
		while(iSendCount) {
		    *hDestination.bHandle = *hSource.bHandle;
			hSource.bHandle++;
			hDestination.bHandle++;
			iSendCount--;
		}
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ControlTransferReadService()                *
 *																 *
 * Parametreler:	 void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon OUT paketi al�nd�ktan sonra    *
 *                   bellek b�lgesine yerle�tirilen verileri han-*
 *                   dle de�eri verilen tampona okur.            *
 *****************************************************************/
void ControlTransferReadService( void )
{
	//Kontrol Write Transfer
	//Cihaz veri basama��nda veri al�r.
	
	unsigned char iRecCount;

	//Son iki biti temizleyerek gelen veri adedini iRecCount'un 
	//alt ve �st byte'lar�na y�kle
	HighLoad(iRecCount, (0x03 & EndPoint0BdtOut.BDnSTAT.BdsArea));
	LowLoad(iRecCount, EndPoint0BdtOut.BDnCNT);
	
	//Geriye kalan okunacak veri adedini iDataCount'a y�kle
	iDataCount += iRecCount;
	
	//Gelen verilerin okunaca�� bellek b�lgesine bir handle al
	hSource.bHandle = (unsigned char*)&CntrlDataPacket;
	
	//Gelen veriler handle de�eri verilmi� bellek b�lgesine okunur
	while(iRecCount)
	{
		*hDestination.bHandle = *hSource.bHandle;
		hSource.bHandle++;
		hDestination.bHandle++;
		iRecCount--;
	}
}

/*****************************************************************
 * Fonksiyon Ismi:   ControlTransferComplateService()            *
 *																 *
 * Parametreler:	 void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon SETUP basama��nda bmRequestType*
 *                   �n 7.biti "1" olmas� halindeki IN i�lemi    *
 *                   sonras� tampona yerle�tirilen verileri g�n- *
 *                   derir, "0" olmas� halindeki OUT i�lemi i�in *
 *                   u�noktay� haz�rlar yada istek desteklenmi-  *
 *                   yorsa STALL g�nderir.                       *
 *****************************************************************/
void ControlTransferComplateService( void )
{
	//Windows'un istekleri cihaz taraf�ndan desteklenmiyorsa bSessionOwner
	//SESSION_NULL olarak kal�r.Bu durumda u�noktan�n STALL ile yan�t vermesi gerekir.
	if(bSessionOwner == SESSION_NULL)
	{
		//U�noktay� bir sonraki setup i�lemi i�in haz�rla
		EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
		EndPoint0BdtOut.ADDR = (unsigned char*)&SetupPacket;
		
		EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_BSTALL;
		EndPoint0BdtIn.BDnSTAT.BdsArea  = _USIE|_BSTALL;
	}
	else
	{
		if(SetupPacket.Direction == DEVICE_TO_HOST)
		{
			//Veriler cihaz'dan PC'ye do�ru gidecekse tampondaki bilgileri
			//g�ndermek i�in ControlTransferWriteService()'� �a��r.(�ift i�lem)
			//Daha sonra u�nokta in-out'u sonraki i�lemler i�in haz�rla.
			if(SetupPacket.wLength < iDataCount) iDataCount = SetupPacket.wLength;    //PC'nin istedi�i kadar veriyi g�nder
			ControlTransferWriteService();
			
			bTransferState = CTRL_SEND;
			
			EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
			EndPoint0BdtOut.ADDR = (unsigned char*)&SetupPacket;
			EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE;                  //SIE mod
			
			EndPoint0BdtIn.ADDR = (unsigned char*)&CntrlDataPacket;
			EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		}
		else
		{
			bTransferState = CTRL_REC;
			
			//E�er i�lem erken sonland�r�l�rsa cihaz PC'ye stat� basama��nda
			//0 uzunluklu bir veri paketi yollayarak transferin ba�ar�s�n� bildirir.
			//Bu i�lem i�in u�nokta0 IN'i yap�land�r.
			EndPoint0BdtIn.BDnCNT = 0;
			EndPoint0BdtIn.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
			
			//Bir sonraki OUT i�lemi i�in u�nokta0'� haz�rla
			EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
			EndPoint0BdtOut.ADDR = (unsigned char*)&CntrlDataPacket;
			EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT1|_DTSEN;
		}
	}
	UCONbits.PKTDIS = false;    //Paket al�m� aktif
}

/*****************************************************************
 * Fonksiyon Ismi:   USBInitNextSetupTrf()                       *
 *															     *
 * Parametreler:     void										 *
 *																 *
 * D�nen De�er:      void										 *
 *																 *
 * A��klama:         Bu fonksiyon u�noktay� bir sonraki SETUP i�-*
 *                   lemi i�in haz�rlar.						 *
 *****************************************************************/
void USBInitNextSetupTrf( void )
{
	bTransferState = WAIT;
	EndPoint0BdtOut.BDnCNT = ENDPOINT0_BUFFER_SIZE;
	EndPoint0BdtOut.ADDR   = (unsigned char*)&SetupPacket;   
	EndPoint0BdtOut.BDnSTAT.BdsArea = _USIE|_DAT0|_DTSEN;    //Data Sync aktif, ilk durum DTS0, ve SIE mod
	EndPoint0BdtIn.BDnSTAT.BdsArea  = _UCPU;                 //U�nokta0 In CPU'da
}