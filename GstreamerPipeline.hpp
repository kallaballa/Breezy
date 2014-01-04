/*
 * GstreamerPipeline.hpp
 *
 *  Created on: Jan 4, 2014
 *      Author: elchaschab
 */

#ifndef GSTREAMERPIPELINE_HPP_
#define GSTREAMERPIPELINE_HPP_

#include <gst/gst.h>
#include <string>
#include <exception>
#include <boost/exception/all.hpp>

namespace breezy {

using std::string;

class pipeline_failed :
  public boost::exception,
  public std::exception
{
public:
	pipeline_failed(string message): what_(message)
  {
  }

  virtual const char *what() const throw()
  {
    return what_.c_str();
  }

private:
  std::string what_;
};

class GstreamerPipeline {
	GstElement* pipeline;
public:
	GstreamerPipeline(string strPipeline) : pipeline(NULL){
		GError *error = NULL;
		this->pipeline = gst_parse_launch(strPipeline.c_str(), &error);
		if(pipeline == NULL) {
			throw pipeline_failed(error->message);
		}
	}

	template<typename T> void set(string elementName, string propertyName, T value) {
		GstElement *e = gst_bin_get_by_name(GST_BIN(pipeline), elementName.c_str());
		g_object_set(e, propertyName.c_str(), value, NULL);
	}

	void set(string elementName, string propertyName, string value) {
		GstElement *e = gst_bin_get_by_name(GST_BIN(pipeline), elementName.c_str());
		g_object_set(e, propertyName.c_str(), value.c_str(), NULL);
	}

	void play() {
		GstMessage *msg;
		GstBus *bus;
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
};


} /* namespace breezy */

#endif /* GSTREAMERPIPELINE_HPP_ */
