#include "PulseMonitorSource.hpp"
#include "GstreamerClient.hpp"
#include "GstreamerServer.hpp"
#include <iostream>
#include <getopt.h>
#include <gst/gst.h>

void printUsage() {
	std::cerr << "Usage: breezy [-d][-v][-p <daemonPort>][-m <index>] <target>..." << std::endl;
	std::cerr << "Options:" << std::endl;
	std::cerr << "\t-d\t\tstart the breezy daemon" << std::endl;
	std::cerr << "\t-v\t\tgenerate verbose output" << std::endl;
	std::cerr << "\t-p <port>\tport to listen/connect to" << std::endl;
	std::cerr << "\t-m <index>\tindex of the monitor source" << std::endl;
}

int main(int argc, char *argv[]) {
	int c;
	int daemonPort = 11111;
	int monitorSourceIndex = 0;
	bool verbose = false;
	bool daemon = false;

	while ((c = getopt(argc, argv, "dvh?p:m:")) != -1) {
		switch (c) {
		case 'd':
			daemon = true;
			break;
		case 'v':
			verbose = true;
			break;
		case 'p':
			daemonPort = atoi(optarg);
			break;
		case 'm':
			monitorSourceIndex = atoi(optarg);
			break;
		case 'h':
			printUsage();
			return 1;
			break;
		case ':':
			printUsage();
			return 1;
			break;
		case '?':
			printUsage();
			return 1;
			break;
		}
	}

	if (verbose)
		gst_debug_set_default_threshold(GST_LEVEL_INFO);
	else
		gst_debug_set_default_threshold(GST_LEVEL_WARNING);

	if (daemon) {
		createGstreamerServer(daemonPort);
	} else {
		std::vector<std::string> targets;

		for(int i=optind; i < argc; ++i) {
			targets.push_back(std::string(argv[i]));
		}

		if(targets.empty()) {
			std::cerr << "missing targets" << std::endl;
			printUsage();
			return 1;
		}

		auto monitors = getPulseMonitorSource();

		if (monitors.size() > 0) {
			std::cerr << "Selected monitor source: " << monitors[monitorSourceIndex] << std::endl;
			createGstreamerClient(monitors[monitorSourceIndex], targets);
			return 0;
		} else {
			std::cerr << "Monitor source not found" << std::endl;
			return 1;
		}
	}
}
