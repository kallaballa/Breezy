#include <gst/gst.h>
#include <iostream>
#include "GstreamerClient.hpp"

void createGstreamerClient (std::string monitorSource, std::string host, int port) {
  GstElement *pipeline;
  GstElement *pulsesrc;
  GstElement *udpsink;
  GstMessage *msg;
  GstBus *bus;
  GError *error = NULL;

  gst_init (0, NULL);

  //pulsesrc name=psrc
  pipeline = gst_parse_launch ("pulsesrc name=psrc ! vorbisenc ! rtpvorbispay ! udpsink name=udpsink", &error);
  if (!pipeline) {
    std::cerr << "Parse error: " << error->message << std::endl;
    return;
  }
  
  pulsesrc = gst_bin_get_by_name (GST_BIN (pipeline), "psrc");
  g_object_set (pulsesrc, "device", monitorSource.c_str(), NULL);
  udpsink = gst_bin_get_by_name (GST_BIN (pipeline), "udpsink");
  g_object_set (udpsink, "host", host.c_str(), NULL);
  g_object_set (udpsink, "port", port, NULL);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  std::cerr << "Pipeline playing" << std::endl;

  bus = gst_element_get_bus (pipeline);

  /* wait until we either get an EOS or an ERROR message. Note that in a real
   * program you would probably not use gst_bus_poll(), but rather set up an
   * async signal watch on the bus and run a main loop and connect to the
   * bus's signals to catch certain messages or all messages */
  msg = gst_bus_poll (bus, GST_MESSAGE_EOS, -1);

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS: {
      std::cerr << "EOS" << std::endl;
      break;
    }
    case GST_MESSAGE_ERROR: {
      GError *err = NULL; /* error to show to users                 */
      gchar *dbg = NULL;  /* additional debug string for developers */

      gst_message_parse_error (msg, &err, &dbg);
      if (err) {
    	std::cerr << "ERROR: " << err->message << std::endl;
        g_error_free (err);
      }
      if (dbg) {
    	std::cerr << "Debug details: " << dbg << std::endl;
        g_free (dbg);
      }
    }
  }
  gst_message_unref (msg);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  gst_object_unref (bus);
}

