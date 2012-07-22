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

#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <bcm2835.h>

#include "Gencade.h"

/* time to wait after each cycle */
#define FRAMEWAIT 20

/* Setup the uinput device */
int setup_uinput_device() {
	int uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
	if (uinp_fd == 0) {
		printf("Unable to open /dev/uinput\n");
		return -1;
	}

	struct uinput_user_dev uinp;
	memset(&uinp, 0, sizeof(uinp)); 
	strncpy(uinp.name, "Gen-to-Keyboard device", strlen("Gen-to-Keyboard device"));
	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;
	uinp.id.product = 1;
	uinp.id.vendor = 1;

	// Setup the uinput device
	ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
	ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
	int i = 0;
	for (i = 0; i < 256; i++) {
		ioctl(uinp_fd, UI_SET_KEYBIT, i);
	}

	/* Create input device into input sub-system */
	write(uinp_fd, &uinp, sizeof(uinp));
	if (ioctl(uinp_fd, UI_DEV_CREATE)) {
		printf("Unable to create UINPUT device.");
		return -1;
	}

	return uinp_fd;
}

/* sends a key event to the virtual device */
void send_key_event(int fd, unsigned int keycode, int keyvalue) {
	struct input_event event;
	gettimeofday(&event.time, NULL);

	event.type = EV_KEY;
	event.code = keycode;
	event.value = keyvalue;

	if (write(fd, &event, sizeof(event)) < 0) {
		printf("Error in key simulation\n");
	}

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd, &event, sizeof(event));
	if (write(fd, &event, sizeof(event)) < 0) {
		printf("Error in key simulation\n");
	}
}

/* checks, if a button on the controller is pressed and sends an event according to the button state. */
void processBtn(uint8_t buttons, uint8_t mask, uint8_t key, int uinh) {
	if ( (buttons & mask) == mask ) {
		send_key_event(uinh, key, 1);
	} else {
		send_key_event(uinh, key, 0);
	}
}


int main(int argc, char *argv[]) {

    buttonstate buttons;

    if (!bcm2835_init())
        return 1;


    /* initialize controller structures with GPIO pin assignments */
	Gencontrollers controllers;
	controllers.clock = RPI_GPIO_P1_07;
	controllers.latch = RPI_GPIO_P1_11;
	controllers.sel1  = RPI_GPIO_P1_21;
	controllers.sel2  = RPI_GPIO_P1_26;
	controllers.data1 = RPI_GPIO_P1_12;
	controllers.data2 = RPI_GPIO_P1_16;

	/* set GPIO pins as input or output pins */
	initializeControllers(&controllers);

	/* intialize virtual input device */
	int uinp_fd;
	if ((uinp_fd = setup_uinput_device()) < 0) {
		printf("Unable to find uinput device\n");
		return -1;
	}

	/* enter the main loop */
	while ( 1 ) {

		/* read states of the buttons */
		updateButtons(&controllers, &buttons);

		/* send an event (pressed or released) for each button */
		/* key events for first controller */
        	processBtn(buttons.buttons1,GEN_A,KEY_X,uinp_fd);
        	processBtn(buttons.buttons1,GEN_B,KEY_Z,uinp_fd);
        	processBtn(buttons.buttons1,GEN_C,KEY_RIGHTSHIFT,uinp_fd);
        	processBtn(buttons.buttons1,GEN_START,KEY_ENTER,uinp_fd);
        	processBtn(buttons.buttons1,GEN_LEFT,KEY_LEFT,uinp_fd);
        	processBtn(buttons.buttons1,GEN_RIGHT,KEY_RIGHT,uinp_fd);
        	processBtn(buttons.buttons1,GEN_UP,KEY_UP,uinp_fd);
        	processBtn(buttons.buttons1,GEN_DOWN,KEY_DOWN,uinp_fd);

		/* key events for second controller */
        	processBtn(buttons.buttons2,GEN_A,KEY_E,uinp_fd);
        	processBtn(buttons.buttons2,GEN_B,KEY_R,uinp_fd);
        	processBtn(buttons.buttons2,GEN_C,KEY_O,uinp_fd);
        	processBtn(buttons.buttons2,GEN_START,KEY_P,uinp_fd);
        	processBtn(buttons.buttons2,GEN_LEFT,KEY_C,uinp_fd);
        	processBtn(buttons.buttons2,GEN_RIGHT,KEY_B,uinp_fd);
        	processBtn(buttons.buttons2,GEN_UP,KEY_F,uinp_fd);
        	processBtn(buttons.buttons2,GEN_DOWN,KEY_V,uinp_fd);

		/* wait for some time to keep the CPU load low */
		delay(FRAMEWAIT);
	}

	/* Destroy the input device */
	ioctl(uinp_fd, UI_DEV_DESTROY);
	/* Close the UINPUT device */
	close(uinp_fd);
}