#include <gst/gst.h>
#include <iostream>
#include <vector>
#include "GstreamerClient.hpp"
#include "GstreamerPipeline.hpp"
#include "Caps.hpp"

namespace breezy {

GstreamerPipeline* makeOpusPipeline(std::string monitorSource, std::string clients) {
	GstreamerPipeline* pipeline = new GstreamerPipeline("pulsesrc name=psrc ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! capsfilter name=capsfilter ! multiudpsink name=mudpsink");
	pipeline->set("psrc", "device", monitorSource);
	pipeline->set("mudpsink", "clients", clients);
	pipeline->set("capsfilter", "caps", Caps::OPUS);
 	return pipeline;
}

GstreamerPipeline* makeVorbisPipeline(std::string monitorSource, std::string clients) {
	GstreamerPipeline* pipeline = new GstreamerPipeline("pulsesrc name=psrc ! audioconvert ! vorbisenc ! rtpvorbispay ! multiudpsink name=mudpsink");
	pipeline->set("psrc", "device", monitorSource);
	pipeline->set("mudpsink", "clients", clients);
 	return pipeline;
}

GstreamerPipeline* makeL16Pipeline(std::string monitorSource, std::string clients) {
	GstreamerPipeline* pipeline = new GstreamerPipeline("pulsesrc name=psrc ! audioconvert ! rtpL16pay ! capsfilter name=capsfilter ! multiudpsink name=mudpsink");
	pipeline->set("psrc", "device", monitorSource);
	pipeline->set("mudpsink", "clients", clients);
	pipeline->set("capsfilter", "caps", Caps::L16);
 	return pipeline;
}

void createGstreamerClient(std::string pulseSource, std::vector<std::string> targets, bool raw) {
	int defaultPort=11111;

	std::string clients;
	bool first = true;

	for(auto target : targets) {
		if(!first)
			clients += ",";

		if(target.find(":") == std::string::npos)
			clients += target + ":" + std::to_string(defaultPort);
		else
			clients += target;

		first = false;
	}

	GstreamerPipeline* pipeline;
	if(raw)
		pipeline = makeL16Pipeline(pulseSource, clients);
	else
		pipeline = makeOpusPipeline(pulseSource, clients);

	pipeline->play();
}
}
