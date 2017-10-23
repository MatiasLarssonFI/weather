CXX=g++
LIBS=`pkg-config jsoncpp --libs` `curl-config --libs`
CXXFLAGS=-std=c++11
INCLUDES=-Ihttp -I.
OBJDIR=../obj
SRCDIR=.
objects := $(patsubst %.cxx,%.o,$(wildcard *.cxx))
objects += $(patsubst %.cxx,%.o,$(wildcard http/*.cxx))
BIN=weather

debug: CXXFLAGS += -g -Wall -Wextra

release: CXXFLAGS += -O2 -s
release: CXX=arm-linux-gnueabihf-g++

debug: $(BIN)
release: $(BIN)

$(BIN): $(objects)
	$(CXX) $(addprefix ${OBJDIR}/,$^) -o $@ $(LIBS)

%.o: %.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o ${OBJDIR}/$@

http/%.o: http/%.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o ${OBJDIR}/$@

.PHONY: clean

dirs:
	mkdir -p ${OBJDIR} && \
	mkdir -p ${OBJDIR}/http

clean: dirs
	rm -f $(OBJDIR)/*.o
