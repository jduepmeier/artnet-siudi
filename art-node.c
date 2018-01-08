#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "easy_config.h"
#include "art-node.h"
#include "network.h"
#include "usb.h"
#include "coreloop.h"

volatile sig_atomic_t abort_signaled = 0;

int usage(char* fn){
	printf("%s - Artnet to usb.\n", fn);
	printf("Usage:\n");
	printf("\t%s <path to config file>", fn);
	return EXIT_FAILURE;
}

int config_artSubUni(const char* category, char* key, char* value, EConfig* econfig, void* user_param) {

	CONFIG* config = (CONFIG*) user_param;
	config->art_subUni = strtoul(value, NULL, 10);

	return 0;
}

int config_artNet(const char* category, char* key, char* value, EConfig* econfig, void* user_param) {

	CONFIG* config = (CONFIG*) user_param;

	config->art_net = strtoul(value, NULL, 10);

	return 0;
}

int config_bindhost(const char* category, char* key, char* value, EConfig* econfig, void* user_param) {

	CONFIG* config = (CONFIG*) user_param;

	config->bindhost = malloc(strlen(value) + 1);
	strncpy(config->bindhost, value, strlen(value) + 1);

	return 0;
}

int parse_config(CONFIG* config, char* filepath) {

	EConfig* econfig = econfig_init(filepath, config);

	unsigned artNetCat = econfig_addCategory(econfig, "artnet");
	unsigned genCat = econfig_addCategory(econfig, "general");

	econfig_addParam(econfig, artNetCat, "net", config_artNet);
	econfig_addParam(econfig, artNetCat, "subuni", config_artSubUni);

	econfig_addParam(econfig, genCat, "bindhost", config_bindhost);

	econfig_parse(econfig);
	econfig_free(econfig);

	return 0;
}

void interrupt_handler() {
	printf("Quitting...\n");
	abort_signaled = 1;
}

int main(int argc, char** argv){
	if(argc < 2){
		exit(usage(argv[0]));
	}

	CONFIG config = {
		.bindhost = strdup("*")
	};

	parse_config(&config, argv[1]);

	signal(SIGINT, interrupt_handler);
	signal(SIGTERM, interrupt_handler);

	//open artnet listener
	config.sockfd = udp_listener(config.bindhost, "6454");
	if(config.sockfd < 0){
		fprintf(stderr, "Failed to open ArtNet listener\n");
		exit(usage(argv[0]));
	}

	// init usb interfaces
	if (!init_ctx(&config)) {
		free(config.bindhost);
		return 10;
	}

	if (!find_usb_device(&config)) {
		free(config.bindhost);
		return 20;
	}

	//run main loop
	art_node(&config);

	free(config.bindhost);
	send_close_device(config.device);
	deinit_ctx(&config);
	return 0;
}
