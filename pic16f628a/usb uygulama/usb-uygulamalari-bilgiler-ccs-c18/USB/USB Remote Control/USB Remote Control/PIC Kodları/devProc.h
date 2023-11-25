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
#include <p18f4550.h>
#include "usbdefs.h"
#include "usbmcr.h"
#include "pubfunc.h"
#include "mem_alloc.h"

//T A N I M L A M A L A R 
#define CMD_INDEX			1
#define DEV_INDEX			0

#define ROLE_PORT 			PORTB
#define ROLE_PORT_TRIS		TRISB

#define ROLE_0				0x00
#define ROLE_1				0x01
#define ROLE_2				0x04
#define ROLE_3				0x08

#define DEVICE_0			0
#define DEVICE_1			1
#define DEVICE_2			2
#define DEVICE_3			3

#endif