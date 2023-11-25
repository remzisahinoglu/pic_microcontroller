/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      interrupt.c - Donaným Kesmeleri              *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *																 *
 * Açýklama:        Bu dosyada donaným kesmeleri oluþtuðunda,    *
 *                  rekli kesmelerin tespiti ve ilgili fonksi-   *
 *                  yonlarýn iþletilmesi ile ilgili iþlemler ta- *
 *                  nýmlýdýr.									 *
 *****************************************************************/
//I N C L U D E 
#include "interrupt.h"

//G L O B A L   D E Ð Ý Þ K E N L E R

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * Dönen Deðer:
 *
 * Açýklama:
 *****************************************************************/
#pragma code HighInterruptVector = 0x08
void HighPriorityInterrupts( void ) {
	_asm goto ResolveHighInterruptSource _endasm
}
#pragma code

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * Dönen Deðer:
 *
 * Açýklama:
 *****************************************************************/
#pragma code LowInterruptVector = 0x18
void LowPriorityInterrupts( void ) {
	_asm goto ResolveLowInterruptSource _endasm
}
#pragma code

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * Dönen Deðer:
 *
 * Açýklama:
 *****************************************************************/
#pragma interrupt ResolveHighInterruptSource
void ResolveHighInterruptSource( void )
{
}

/*****************************************************************
 * Fonksiyon Ismi:
 *
 * Parametreler:
 *
 * Dönen Deðer:
 *
 * Açýklama:
 *****************************************************************/
#pragma interruptlow ResolveLowInterruptSource
void ResolveLowInterruptSource( void )
{
}

