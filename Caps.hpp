/*
 * Caps.hpp
 *
 *  Created on: Jan 4, 2014
 *      Author: elchaschab
 */

#ifndef CAPS_HPP_
#define CAPS_HPP_

namespace breezy {

#include <gst/gst.h>

struct Caps {
	static GstCaps* OPUS;
	static GstCaps* VORBIS;
	static GstCaps* L16;
};
} /* namespace breezy */

#endif /* CAPS_HPP_ */
