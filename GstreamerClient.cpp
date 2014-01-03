#include <gst/gst.h>
#include <iostream>
#include <vector>
#include "GstreamerClient.hpp"

GstElement* makeOpusPipeline(std::string monitorSource, std::string clients, GError *error) {
	GstElement *pulsesrc;
	GstElement *mudpsink;
	GstElement *capsfilter;
	GstCaps *caps = gst_caps_new_simple("application/x-rtp",
					"media", G_TYPE_STRING, "audio",
					"payload", G_TYPE_INT, 96,
					"clock-rate", G_TYPE_INT, 48000,
					"encoding-name", G_TYPE_STRING, "X-GST-OPUS-DRAFT-SPITTKA-00",
					"encoding-params", G_TYPE_STRING, "1",
					NULL);

	GstElement *pipeline = gst_parse_launch("pulsesrc name=psrc ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! capsfilter name=capsfilter ! multiudpsink name=mudpsink", &error);
	if(pipeline != NULL) {
		pulsesrc = gst_bin_get_by_name(GST_BIN(pipeline), "psrc");
		g_object_set(pulsesrc, "device", monitorSource.c_str(), NULL);

		mudpsink = gst_bin_get_by_name(GST_BIN(pipeline), "mudpsink");
		g_object_set(mudpsink, "clients", clients.c_str(), NULL);

		capsfilter = gst_bin_get_by_name(GST_BIN(pipeline), "capsfilter");
		g_object_set(capsfilter, "caps", caps, NULL);
	}
 	return pipeline;
}

/*GstElement* makeVorbisPipeline(std::string monitorSource, std::string clients, GError *error) {
	GstElement *pulsesrc;
	GstElement *mudpsink;
	GstCaps *caps = gst_caps_new_simple("application/x-rtp",
					"media", G_TYPE_STRING, "audio",
					"payload", G_TYPE_INT, 96,
					"clock-rate", G_TYPE_INT, 44100,
					"encoding-name", G_TYPE_STRING, "VORBIS",
					"encoding-params", G_TYPE_STRING, "1",
					"configuration", G_TYPE_STRING, "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\\=\\=",
					NULL);

	GstElement *pipeline = gst_parse_launch("pulsesrc name=psrc ! audioconvert ! vorbisenc ! rtpvorbispay ! multiudpsink name=mudpsink", &error);
	if(pipeline != NULL) {
		pulsesrc = gst_bin_get_by_name(GST_BIN(pipeline), "psrc");
		g_object_set(pulsesrc, "device", monitorSource.c_str(), NULL);
		mudpsink = gst_bin_get_by_name(GST_BIN(pipeline), "mudpsink");
		g_object_set(mudpsink, "clients", clients.c_str(), NULL);
		g_object_set(mudpsink, "caps", caps, NULL);
	}
 	return pipeline;
}*/

GstElement* makeL16Pipeline(std::string monitorSource, std::string clients, GError *error) {
	GstElement *pulsesrc;
	GstElement *mudpsink;
	GstCaps *caps = gst_caps_new_simple("application/x-rtp",
					"media", G_TYPE_STRING, "audio",
					"payload", G_TYPE_INT, 96,
					"clock-rate", G_TYPE_INT, 44100,
					"encoding-name", G_TYPE_STRING, "L16",
					"encoding-params", G_TYPE_STRING, "2",
					"channels", G_TYPE_INT, 2,
					NULL);

	GstElement *pipeline = gst_parse_launch("pulsesrc name=psrc ! audioconvert ! rtpL16pay ! multiudpsink name=mudpsink", &error);
	if(pipeline != NULL) {
		pulsesrc = gst_bin_get_by_name(GST_BIN(pipeline), "psrc");
		g_object_set(pulsesrc, "device", monitorSource.c_str(), NULL);
		mudpsink = gst_bin_get_by_name(GST_BIN(pipeline), "mudpsink");
		g_object_set(mudpsink, "clients", clients.c_str(), NULL);
		g_object_set(mudpsink, "caps", caps, NULL);
	}
	return pipeline;
}

void createGstreamerClient(std::string pulseSource, std::vector<std::string> targets, bool raw) {
	gst_init(0, NULL);
	int defaultPort=11111;
	GstElement *pipeline;
	GstElement *pulsesrc;
	GstElement *mudpsink;
	GstMessage *msg;
	GstBus *bus;
	GError *error = NULL;

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

	if(raw)
		pipeline = makeL16Pipeline(pulseSource, clients, error);
	else
		pipeline = makeOpusPipeline(pulseSource, clients, error);

	if (!pipeline) {
		std::cerr << "Parse error: " << error->message << std::endl;
		return;
	}

	gst_element_set_state(pipeline, GST_STATE_PLAYING);
	std::cerr << "Pipeline playing" << std::endl;

	bus = gst_element_get_bus(pipeline);

	/* wait until we either get an EOS or an ERROR message. Note that in a real
	 * program you would probably not use gst_bus_poll(), but rather set up an
	 * async signal watch on the bus and run a main loop and connect to the
	 * bus's signals to catch certain messages or all messages */
	msg = gst_bus_poll(bus, GST_MESSAGE_ERROR, -1);

	switch (GST_MESSAGE_TYPE(msg)) {
	case GST_MESSAGE_EOS: {
		std::cerr << "EOS" << std::endl;
		break;
	}
	case GST_MESSAGE_ERROR: {
		GError *err = NULL; /* error to show to users                 */
		gchar *dbg = NULL; /* additional debug string for developers */

		gst_message_parse_error(msg, &err, &dbg);
		if (err) {
			std::cerr << "ERROR: " << err->message << std::endl;
			g_error_free(err);
		}
		if (dbg) {
			std::cerr << "Debug details: " << dbg << std::endl;
			g_free(dbg);
		}
	}
	}

	gst_message_unref(msg);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	gst_object_unref(bus);
}
