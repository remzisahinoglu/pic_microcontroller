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
 * A��klama:        Bu dosyada USB ram b�lgesi, mem_alloc.h dos- *
 *                  yas�nda tan�ml� yap�lar ile b�l�mlendiril-   *
 *                  mi�tir.                                      *
 *****************************************************************/
//I N C L U D E S
#include "mem_alloc.h"

//G L O B A L   D E � � � K E N L E R 
unsigned char bTransferState;
unsigned char bHidReportLength;
unsigned char bDeviceState;
unsigned char bActiveConfiguration;
unsigned char bAlternateInterface[MAX_INTERFACE_NUM];
unsigned char bSessionOwner;
unsigned int  iDataCount;

HANDLE hSource;
HANDLE hDestination;
DEVICE_STATUS UsbDeviceStatus;

//U S B   R A M   Y E R L E � � M �
#pragma udata UsbMemAlloc = 0x400

//Buffer Descriptor Table
volatile far BUFF_DESC_TAB  EndPoint0BdtOut;
volatile far BUFF_DESC_TAB	EndPoint0BdtIn;
volatile far BUFF_DESC_TAB	EndPoint1BdtOut;
volatile far BUFF_DESC_TAB	EndPoint1BdtIn;

//U�nokta0 Tampon yerle�imi
volatile far SETUP_PACKET   SetupPacket;
volatile far DATA_PACKET	CntrlDataPacket;

//HID Veri Tamponu
volatile far unsigned char  HidReportOut[HID_OUT_ENDPT_SIZE];
volatile far unsigned char  HidReportIn[HID_IN_ENDPT_SIZE];
volatile far unsigned char  HidFeatureReport[ENDPOINT0_BUFFER_SIZE];

