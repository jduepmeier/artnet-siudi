#pragma once

#include <inttypes.h>
#include <signal.h>

extern volatile sig_atomic_t abort_signaled;;

#define DMX_CHANNELS 16
typedef struct /*XLASER_CFG*/ {
	uint8_t art_net;
	uint8_t art_subUni;
	struct libusb_device_handle* device;
	struct libusb_context* ctx;
	char* bindhost;
	int sockfd;
} CONFIG;
