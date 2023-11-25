/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      pubfunc.h - Public Fonksiyonlar              *
 * 																 *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *****************************************************************
 * Açýklama:        Bu dosyada tüm dosyalar tarafýndan çaðrýla-  *
 *                  bilen açýk fonksiyon prototipleri bulunmak-  *
 *                  tadýr.	                                     *
 *****************************************************************/
#ifndef PUBFUNC_H
#define PUCFUNC_H

void UserInit( void );
void USBService( void );
void USBCheckBus( void );
void EndPointService( void );
void CheckHidRequest( void );
void HidInitEndpoint( void );
void USBInitNextSetupTrf( void );
void CheckStandartRequest( void );
void ControlTransferReadService( void );
void UserProc( unsigned char Direction );
void ControlTransferWriteService( void );
void ControlTransferComplateService( void );
void FlushBuffer(unsigned char *buffer, unsigned char length);
void HidTransferWriteService(char *buffer, unsigned char Length);
unsigned char HidTransferReadService(char *buffer, unsigned char Length);

#endif