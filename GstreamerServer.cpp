#include <gst/gst.h>
#include <iostream>
#include <string>
#include "GstreamerServer.hpp"

void createGstreamerServer (int aport, int vport) {
	using std::string;
  GstElement *pipeline;
  GstElement *asrc;
  GstElement *vsrc;
  GstMessage *msg;
  GstBus *bus;
  GError *error = NULL;
  GstCaps *acaps = gst_caps_new_simple ("application/x-rtp",
          "media", G_TYPE_STRING, "audio",
          "payload",G_TYPE_INT,96,
          "clock-rate", G_TYPE_INT, 44100,
          "encoding-name", G_TYPE_STRING,"VORBIS",
          "encoding-params", G_TYPE_STRING,"1",
          "ssrc", G_TYPE_INT, 2504844413,
          "clock-base", G_TYPE_INT, 17980830,
          "seqnum-base", G_TYPE_INT, 41855,
          "configuration", G_TYPE_STRING, "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sThAoSErAYBwAADAGKWYcgw6CSk1jDkGoZSUUmqtYYwxCKWk1FpLlXMQSkmptdhirJyDUFJKrcUaYwchpdZarLHWmjsIKaUWa6w52BxKaS3GWHPOvfeQUmsx1lpz772X1mKsNefcgxDCtBRjrrn24HvvKbZaa809+CCEULHVWnPwQQghhIsx99yD8D0IIVyMOecehPDBB2EAAHeDAwBEgo0zrCSdFY4GFxqyEgAICQAgEGKKMeecgxBCCJFSjDnnHIQQQiglUoox55yDDkIIJWSMOecchBBCKKWUjDHnnIMQQgmllJI55xyEEEIopZRSMueggxBCCaWUUkrnHIQQQgillFJK6aCDEEIJpZRSSikhhBBCCaWUUkopJYQQQgmllFJKKaWEEEoopZRSSimllBBCKaWUUkoppZQSQiillFJKKaWUkkIppZRSSimllFJSKKWUUkoppZRSSgmllFJKKaWUlFJJBQAAHDgAAAQYQScZVRZhowkXHoBCQ1YCAEAAABTEVlOJnUHMMWepIQgxqKlCSimGMUPKIKYpUwohhSFziiECocVWS8UAAAAQBAAICAkAMEBQMAMADA4QPgdBJ0BwtAEACEJkhkg0LASHB5UAETEVACQmKOQCQIXFRdrFBXQZ4IIu7joQQhCCEMTiAApIwMEJNzzxhifc4ASdolIHAQAAAABwAAAPAADHBRAR0RxGhsYGR4fHB0hIAAAAAADIAMAHAMAhAkRENIeRobHB0eHxARISAAAAAAAAAAAABAQEAAAAAAACAAAABAQ\\=",
          NULL);

  GstCaps *vcaps = gst_caps_new_simple ("application/x-rtp",
		  "media", G_TYPE_STRING, "video",
		  "payload", G_TYPE_INT, 96,
		  "clock-rate", G_TYPE_INT, 90000,
		  "encoding-name", G_TYPE_STRING, "MP4V-ES",
		  "profile-level-id", G_TYPE_STRING, "1",
		  "config", G_TYPE_STRING, "000001b001000001b58913000001000000012000c48d8800cd3204709463000001b24c61766335322e3132332e30",
		  "ssrc", G_TYPE_INT, 3539298592,
		  "clock-base", G_TYPE_INT, 3824132269,
		  "seqnum-base", G_TYPE_INT, 1000,
		  NULL);

  gst_init (0, NULL);
  gst_debug_set_default_threshold(GST_LEVEL_INFO);

  pipeline = gst_parse_launch ("udpsrc name=asrc ! rtpvorbisdepay ! vorbisdec ! autoaudiosink \
		  udpsrc name=vsrc ! rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink", &error);
  if (!pipeline) {
    std::cerr << "Parse error: " << error->message << std::endl;
    return;
  }

  asrc = gst_bin_get_by_name (GST_BIN (pipeline), "asrc");
  vsrc = gst_bin_get_by_name (GST_BIN (pipeline), "vsrc");

  g_object_set (G_OBJECT(asrc),"caps", acaps, NULL);
  g_object_set (asrc, "port", aport, NULL);

  g_object_set (G_OBJECT(vsrc),"caps", vcaps, NULL);
  g_object_set (vsrc, "port", vport, NULL);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  std::cerr << "Pipeline playing" << std::endl;

  bus = gst_element_get_bus (pipeline);

  msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, -1);

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

