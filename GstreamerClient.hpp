#ifndef GSTREAMERCLIENT_HPP_
#define GSTREAMERCLIENT_HPP_

#include <string>

namespace breezy {
void createGstreamerClient(std::string monitorSource, std::vector<std::string> targets, bool raw=false);
}

#endif
