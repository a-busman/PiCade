/*
 * NEScade - Simulates a virtual keyboard for two NES controllers that are 
 * connected to the GPIO pins of the Raspberry Pi.
 *
 * (c) Copyright 2012  Alex Busman (alex.busman@gmail.com)
 *
 *
 * Permission to use, copy, modify and distribute NEScade in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * NEScade is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for NEScade or software derived from NEScade.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so everyone can benefit from the modifications
 * in future versions.
 *
 * Raspberry Pi is a trademark of the Raspberry Pi Foundation.
 */

#include "NEScade.h"

/* set the GPIO pins as input or output pins */
void initializeControllers(NEScontrollers *controllers) {
  printf("Initializing NES controller.\n");
  bcm2835_gpio_fsel( controllers->latch, BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( controllers->clock, BCM2835_GPIO_FSEL_OUTP );
  bcm2835_gpio_fsel( controllers->data1, BCM2835_GPIO_FSEL_INPT );
  bcm2835_gpio_fsel( controllers->data2, BCM2835_GPIO_FSEL_INPT );
  bcm2835_gpio_write( controllers->latch, LOW );
  bcm2835_gpio_write( controllers->clock, LOW );
}

/* check the state of each button */
void updateButtons(NEScontrollers *controllers, buttonstate* buttons) {
  bcm2835_gpio_write( controllers->latch, HIGH );
  delayMicroseconds(2);
  bcm2835_gpio_write( controllers->latch, LOW );
  delayMicroseconds(2);

  buttons->buttons1 = 0;
  buttons->buttons2 = 0;
  int i;
  for (i = 0; i < 8; i++) {

    uint8_t curpin1 = bcm2835_gpio_lev(controllers->data1);
    uint8_t curpin2 = bcm2835_gpio_lev(controllers->data2);
    bcm2835_gpio_write( controllers->clock, HIGH );
    delayMicroseconds(2);
    bcm2835_gpio_write( controllers->clock, LOW );
    delayMicroseconds(2);

    if( curpin1==LOW ) {
      buttons->buttons1 |= (1<<i);
    }
    if( curpin2==LOW ) {
      buttons->buttons2 |= (1<<i);
    }
  }
}
