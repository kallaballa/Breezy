#include "PulseMonitorSource.hpp"
#include "GstreamerClient.hpp"
#include "GstreamerServer.hpp"
#include <iostream>
#include <getopt.h>

void printUsage() {
	std::cerr << "Usage: breezy [-d] <host>" << std::endl;
}

int main(int argc, char *argv[]) {
	int c;
	int port=11111;

	bool daemon = false;
    while ((c = getopt(argc, argv, "dp:")) != -1) {
      switch (c) {
      case 'd':
    	daemon = true;
        break;
      case 'p':
    	port = atoi(optarg);
        break;
      case 'h':
        printUsage();
        break;
      case ':':
        printUsage();
        break;
      case '?':
        printUsage();
        break;
      }
    }

    if(daemon) {
    	createGstreamerServer(port);
    } else {
		if ((argc - optind) == 1) {
			std::string host = argv[optind];
			auto monitors = getPulseMonitorSource();

			if (monitors.size() > 0) {
				for (auto m : monitors) {
					std::cout << m << std::endl;
				}

				createGstreamerClient(monitors[0], host, port);
				return 0;
			} else {
				return 1;
			}
		} else {
			std::cerr << "missing hostname" << std::endl;
			printUsage();
		}
    }
}
