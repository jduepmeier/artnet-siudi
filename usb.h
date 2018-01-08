#pragma once

#include <libusb-1.0/libusb.h>
#include <stdbool.h>
#include "art-node.h"

bool setup_device(libusb_device_handle* handle, unsigned universe);

bool open_device(libusb_device_handle* handle);
int send_dmx(libusb_device_handle* handle, unsigned char* data, unsigned len);
void send_close_device(libusb_device_handle* handle);
bool init_ctx(CONFIG* config);

void close_usb_device(CONFIG* config);

void deinit_ctx(CONFIG* config);

bool find_usb_device(CONFIG* config);
