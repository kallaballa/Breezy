CXX     := g++-4.8
TARGET  := breezy
SRCS    := breezy.cpp PulseMonitorSource.cpp  GstreamerClient.cpp GstreamerServer.cpp
OBJS    := ${SRCS:.cpp=.o} 
DEPS    := ${SRCS:.cpp=.dep} 
    
CXXFLAGS = -pedantic -std=c++11 -g -O0 `pkg-config --cflags gstreamer-0.10`
LDFLAGS = 
LIBS    = -lpulse `pkg-config --libs gstreamer-0.10`

.PHONY: all clean

ifeq ($(UNAME), Darwin)
 CXXFLAGS +=  -stdlib=libc++
endif

all: release

release: ${TARGET}

${TARGET}: ${OBJS} 
	${CXX} ${LDFLAGS} -o $@ $^ ${LIBS} 

${OBJS}: %.o: %.cpp %.dep 
	${CXX} ${CXXFLAGS} -o $@ -c $< 

${DEPS}: %.dep: %.cpp Makefile 
	${CXX} ${CXXFLAGS} -MM $< > $@ 

clean:
	rm -f *~ *.dep *.o ${TARGET} 

distclean: clean

