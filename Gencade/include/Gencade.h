/*
 * Gencade - Simulates a virtual keyboard for two Genesis controllers that are 
 * connected to the GPIO pins of the Raspberry Pi.
 *
 * (c) Copyright 2012  Alex Busman (alex.busman@gmail.com)
 *
 *
 * Permission to use, copy, modify and distribute Gencade in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Gencade is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for Gencade or software derived from Gencade.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so everyone can benefit from the modifications
 * in future versions.
 *
 * Raspberry Pi is a trademark of the Raspberry Pi Foundation.
 */

#ifndef Gencade_h
#define Gencade_h

#include <inttypes.h>
#include <bcm2835.h>
#include <unistd.h>
#include <stdio.h>

/* holds the GPIO pins for the clock, latch and data signals */
typedef struct{
	RPiGPIOPin clock;
	RPiGPIOPin latch;
	RPiGPIOPin sel1;
	RPiGPIOPin sel2;
	RPiGPIOPin data1;
	RPiGPIOPin data2;
} Gencontrollers;

/* holds the button staes for the first and second controller */
typedef struct {
	uint8_t buttons1;
	uint8_t buttons2;
} buttonstate;

/* bit masks for checking the button states */
#define GEN_UP      0x08
#define GEN_DOWN    0x04
#define GEN_A       0x02
#define GEN_START   0x01
#define GEN_LEFT    0x80
#define GEN_RIGHT   0x40
#define GEN_B       0x20
#define GEN_C	    0x10


/* check the state of each button */
void updateButtons(Gencontrollers *controllers, buttonstate *buttons);

/* set the GPIO pins as input or output pins */
void initializeControllers(Gencontrollers *controllers);

#endif
