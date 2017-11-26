CXX=g++
LIBS=-lcurl -lz -lssl -lcrypto
CXXFLAGS=-std=c++11
INCLUDES=-Ihttp
OBJDIR=../obj
SRCDIR=.
BINDIR=../bin/debug
objects := $(patsubst %.cxx,%.o,$(wildcard *.cxx))
objects := $(objects) $(patsubst %.cxx,%.o,$(wildcard http/*.cxx))
BIN=weather

debug: CXXFLAGS += -g -Wall -Wextra

release: CXX=arm-linux-gnueabihf-g++
release: CXXFLAGS += -O2
release: BINDIR=../bin/release

debug: $(BIN)
release: $(BIN)

$(BIN): $(objects)
	$(CXX) $(addprefix ${OBJDIR}/,$^) -o $(BINDIR)/$@ $(LIBS)

%.o: %.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o ${OBJDIR}/$@

http/%.o: http/%.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o ${OBJDIR}/$@

.PHONY: clean

dirs:
	mkdir -p ${OBJDIR} && \
	mkdir -p ${OBJDIR}/http
	mkdir -p ../bin/debug
	mkdir -p ../bin/release

clean: dirs
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/http/*.o
	rm -f $(BINDIR)/*
