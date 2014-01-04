#include <gst/gst.h>
#include <iostream>
#include <string>
#include "GstreamerServer.hpp"
#include "GstreamerPipeline.hpp"
#include "Caps.hpp"

namespace breezy {
GstreamerPipeline* makeOpusPipeline(int port) {
	GstreamerPipeline* pipeline = new GstreamerPipeline("udpsrc name=udpsrc ! gstrtpjitterbuffer name=jitterbuf ! rtpopusdepay ! opusdec name=opusdec ! audioconvert ! autoaudiosink");
	pipeline->set("udpsrc", "port", port);
	pipeline->set("udpsrc", "caps", Caps::OPUS);
	pipeline->set("jitterbuf", "latency", 250);
	pipeline->set("jitterbuf", "do-lost", true);
	pipeline->set("opusdec", "plc", true);
	return pipeline;
}

GstreamerPipeline* makeVorbisPipeline(int port) {
	GstreamerPipeline* pipeline = new GstreamerPipeline("udpsrc name=udpsrc ! rtpvorbisdepay ! vorbisdec ! audioconvert ! autoaudiosink");
	pipeline->set("udpsrc", "port", port);
	pipeline->set("udpsrc", "caps", Caps::VORBIS);
	return pipeline;
}

GstreamerPipeline* makeL16Pipeline(int port) {
	GstreamerPipeline* pipeline = new GstreamerPipeline("udpsrc name=udpsrc ! gstrtpjitterbuffer name=jitterbuf ! rtpL16depay ! audioconvert ! autoaudiosink");
	pipeline->set("udpsrc", "port", port);
	pipeline->set("udpsrc", "caps", Caps::L16);
	pipeline->set("jitterbuf", "latency", 250);
	pipeline->set("jitterbuf", "do-lost", true);
	return pipeline;
}

void createGstreamerServer(int port, bool raw) {
	gst_init(0, NULL);
	using std::string;
	GstreamerPipeline *pipeline;

	if(raw)
		pipeline = makeL16Pipeline(port);
	else
		pipeline = makeOpusPipeline(port);

	pipeline->play();
}
}
