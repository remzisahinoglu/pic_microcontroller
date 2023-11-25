/*****************************************************************
 *             U S B   H I D   U Y G U L A M A S I               * 
 *****************************************************************
 * Dosya Ismi:      devProc.h - Cihaz Görevleri Tanýmlamalarý    *
 * 															     *
 * Version:         1.00                                         *
 *																 *
 * Tarih:           06.05.2007                                   *
 *																 *
 * Author:          Ahmet ATAR								     *
 *																 *
 * Açýklama:                                                     *
 *****************************************************************/
#ifndef DEVPROC_H
#define DEVPROC_H

//I N C L U D E S 
#include "usbdefs.h"
#include "usbmcr.h"
#include "pubfunc.h"
#include "mem_alloc.h"

//Ö Z E L  F O N K S I Y O N   P R O T O T I P
//None

#define PWM_PIN				   PORTCbits.RC2
#define PWM_TRIS			   TRISCbits.TRISC2
#define PWM_MODE			   0x0C
#define OPEN_MOTOR				 25
#define CLOSE_MOTOR				 26
#define CMD_OPEN_CLOSE			  0

#define mInitPwm() {      \
	PWM_TRIS = false;     \
	CCP2CON  = PWM_MODE;  \
	PR2		 = 0xff;      \
}

#define mStartPwmOfTimer()      { T2CONbits.TMR2ON = true; }
#define mStopPwmOfTimer()       { T2CONbits.TMR2ON = false;}

#endif