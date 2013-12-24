CXX     := g++
TARGET  := breezy
SRCS    := breezy.cpp PulseMonitorSource.cpp  GstreamerClient.cpp GstreamerServer.cpp
OBJS    := ${SRCS:.cpp=.o} 
DEPS    := ${SRCS:.cpp=.dep} 
    
CXXFLAGS = -pedantic -std=c++11 `pkg-config --cflags libpulse` `pkg-config --cflags gstreamer-0.10`
LDFLAGS = 
LIBS    =  `pkg-config --libs libpulse` `pkg-config --libs gstreamer-0.10`

.PHONY: all clean

ifeq ($(UNAME), Darwin)
 CXXFLAGS +=  -stdlib=libc++
endif

all: release

debug: CXXFLAGS+=-g -O0
debug: ${TARGET}

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

