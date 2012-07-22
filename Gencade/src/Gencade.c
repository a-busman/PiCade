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

#include "Gencade.h"

/* set the GPIO pins as input or output pins */
void initializeControllers(Gencontrollers *controllers) {
  printf("Initializing Genesis controller.\n");
  bcm2835_gpio_fsel( controllers->latch, BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( controllers->clock, BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( controllers->sel1,  BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( controllers->sel2,  BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( controllers->data1, BCM2835_GPIO_FSEL_INPT );
  bcm2835_gpio_fsel( controllers->data2, BCM2835_GPIO_FSEL_INPT );
  bcm2835_gpio_write( controllers->latch, LOW );
  bcm2835_gpio_write( controllers->clock, LOW );
}

/* check the state of each button */
void updateButtons(Gencontrollers *controllers, buttonstate* buttons) {

  bcm2835_gpio_write( controllers->sel1, LOW );
  bcm2835_gpio_write( controllers->sel1, LOW );

  buttons->buttons1 = 0;
  buttons->buttons2 = 0;
  int i,j,pos;
  pos=0;
  for (i = 0; i < 2; i++) {
	  bcm2835_gpio_write( controllers->latch, HIGH );
	  delayMicroseconds(2);
	  bcm2835_gpio_write( controllers->latch, LOW );
	  delayMicroseconds(2);
	  for (j = 0; j < 6; j++) {

		uint8_t curpin1 = bcm2835_gpio_lev(controllers->data1);
		uint8_t curpin2 = bcm2835_gpio_lev(controllers->data2);
		bcm2835_gpio_write( controllers->clock, HIGH );
		delayMicroseconds(2);
		bcm2835_gpio_write( controllers->clock, LOW );
		delayMicroseconds(2);
		if( curpin1==LOW && (i!=0 || (j!=2 && j!=3))) {
		  buttons->buttons1 |= (1<<pos);
		}
		if( curpin2==LOW && (i!=0 || (j!=2 && j!=3))) {
		  buttons->buttons2 |= (1<<pos);
		}
		if(i!=0 || ( j!=2 && j!=3 ) ) {
			pos++;
		}
	  }
	  bcm2835_gpio_write( controllers->sel1, HIGH );
      	  bcm2835_gpio_write( controllers->sel2, HIGH );
  }
}
