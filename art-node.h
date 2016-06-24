#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <ctype.h>
//#include <math.h>
//#include <errno.h>


volatile sig_atomic_t abort_signaled = 0;

#define DMX_CHANNELS 16
typedef struct /*XLASER_CFG*/ {
	uint8_t art_net;
	uint8_t art_subUni;
	struct libusb_device_handle* device;
	struct libusb_context* ctx;
	char* bindhost;
	int sockfd;
} CONFIG;


// CHANNELS
// X 16bit
// Y 16bit
// R G B 24bit
// Dimmer
// Shutter
// Gobo
// Focus
// Rotation Abs
// Rotation Speed
#include "easy_config.h"
#include "easy_config.c"
#include "network.h"
#include "usb.c"
#include "artnet.h"
#include "artnet.c"
#include "coreloop.c"
