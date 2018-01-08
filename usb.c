#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>

#include "art-node.h"

bool setup_device(libusb_device_handle* handle, unsigned universe) {

	unsigned char ans[512];
	int rc;
	memset(ans, 0, 512);
	ans[0] = universe;

	rc = libusb_control_transfer(handle, 0x40, 48, 0xffff, 0, ans, 0, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}

	rc = libusb_control_transfer(handle, 0x40, 17, 0x0001, 0, ans, 0, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}
	rc = libusb_control_transfer(handle, 0xC0, 33, 0x0000, 1, ans, 1, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}

	rc = libusb_control_transfer(handle, 0x40, 33, 0x0000, 1, ans, 1, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}

	return true;
}

bool open_device(libusb_device_handle* handle) {
	unsigned char buf[1024];
	memset(buf, 0, sizeof(buf));
	int rc;

	rc = libusb_control_transfer(handle, 0xC0, 33, 0x0000, 1, buf, 1, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}

	rc = libusb_control_transfer(handle, 0xC0, 33, 0x0000, 0, buf, 64, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}

	rc = libusb_control_transfer(handle, 0xC0, 33, 0x0001, 0, buf, 64, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}
	// i don't know what this is, but it works :)
	unsigned char dat[128] = {0xd4, 0xf5, 0x15, 0x82, 0x12, 0x37, 0x99, 0x4f, 0x10, 0xbf, 0x34,
						0xdd, 0x9b, 0x00, 0x0a, 0x74, 0x2a, 0xe5, 0xbe, 0x7c, 0xe1, 0xac,
						0x72, 0xe4, 0x8a, 0xe7, 0xd0, 0x75, 0x9b, 0x79, 0xec, 0xb0, 0x61,
						0xb5, 0x07, 0xdf, 0x27, 0x11, 0x90, 0xef, 0xdc, 0x9b, 0x0c, 0x36,
						0x8f, 0xb8, 0x0b, 0xc0, 0x82, 0x34, 0x66, 0x6c, 0xc5, 0x9b, 0x88,
						0xd7, 0x78, 0x23, 0x2a, 0x74, 0x9f, 0x3b, 0xbe, 0xb7, 0x4b, 0x44,
						0x7c, 0x9d, 0xe1, 0x70, 0xf8, 0xec, 0x4a, 0x13, 0x74, 0x1d, 0xfe,
						0x03, 0xce, 0x91, 0xb3, 0x74, 0xff, 0xf2, 0x0f, 0x34, 0x2e, 0xb2,
						0x40, 0xb1, 0x67, 0x26, 0xa8, 0x52, 0x79, 0x3b, 0xf4, 0x79, 0x37,
						0xb8, 0xbf, 0xf7, 0x79, 0x48, 0x61, 0xaf, 0xa9, 0xf5, 0x52, 0x2a,
						0xbb, 0x39, 0x98, 0xd8, 0xd8, 0x78, 0x49, 0xde, 0x80, 0x63, 0x75,
						0x44, 0xd0, 0xd3, 0xbd, 0x5d, 0x61, 0x86};
	rc = libusb_control_transfer(handle, 0x40, 33, 0x0000, 0, dat, 128, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}
	rc = libusb_control_transfer(handle, 0xC0, 33, 0x0000, 1, buf, 1, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}
	rc = libusb_control_transfer(handle, 0x40, 6, 0x0002, 0, buf, 0, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}
	rc = libusb_control_transfer(handle, 0xC0, 2, 0x0000, 0, buf, 64, 0);

	if (rc < 0 && rc != LIBUSB_ERROR_PIPE) {
		fprintf(stderr, "%s\n", libusb_strerror(rc));
		return false;
	}
	return true;
}

int send_dmx(libusb_device_handle* handle, unsigned char* data, unsigned len) {

		int transfered;
		unsigned char ans[512];
		libusb_control_transfer(handle, 0xC0, 33, 0x0000, 1, ans, 1, 0);

		libusb_bulk_transfer(handle, 0x02, data, len, &transfered, 0);
		printf("Bytes send: %d\n", transfered);

		return 0;
}

void send_close_device(libusb_device_handle* handle) {

	if (handle) {
		libusb_control_transfer(handle, 0x40, 6, 0x0003, 0, NULL, 0, 100);
	}
}

bool init_ctx(CONFIG* config) {

	int rc = libusb_init(&config->ctx);

	if (rc != 0) {
		fprintf(stderr, "Cannot initialize libusb context.\n");
		return false;
	}
	libusb_set_debug(config->ctx, LIBUSB_LOG_LEVEL_WARNING);

	return true;
}

void close_usb_device(CONFIG* config) {
	if (config->device) {
		libusb_release_interface(config->device, 0x0);
		libusb_close(config->device);
	}
}

void deinit_ctx(CONFIG* config) {
	close_usb_device(config);
	if (config->ctx) {
		libusb_exit(config->ctx);
	}
}

bool find_usb_device(CONFIG* config) {
	const uint16_t idVendor = 0x6244;
	const uint16_t idProduct9A = 0x0441;
	const uint16_t idProduct8A = 0x0421;
	if (!config->ctx) {
		fprintf(stderr, "libusb is not initialized.\n");
		return false;
	}

	config->device = libusb_open_device_with_vid_pid(config->ctx, idVendor, idProduct9A);

	if (!config->device) {
		config->device = libusb_open_device_with_vid_pid(config->ctx, idVendor, idProduct8A);
	}

	if (!config->device) {
		fprintf(stderr, "cannot find device.\n");
		return false;
	}

	// check if device kernel driver is active
	if(libusb_kernel_driver_active(config->device, 0) == 1) {
		if (libusb_detach_kernel_driver(config->device, 0) != 0) {
			fprintf(stderr, "Cannot detach kernel driver.\n");
			libusb_close(config->device);
			config->device = NULL;
			return false;
		}
	}
	if (libusb_claim_interface(config->device, 0) < 0) {
		fprintf(stderr, "Cannot claim the interface.\n");
		libusb_close(config->device);
		config->device = NULL;
		return false;
	}
	if (!open_device(config->device)) {
		close_usb_device(config);
		return false;
	}
	if (!setup_device(config->device, 1)) {
		close_usb_device(config);
		return false;
	}

	return true;
}
