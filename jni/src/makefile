CXX := g++
LIBRARY := libjcltune.dylib
OBJFILES := JTuner.o
INCLUDES := -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin -I../include
CXXFLAGS := $(INCLUDES) -std=c++11

$(LIBRARY): $(OBJFILES)
	$(CXX) -shared -o ../../lib/JCLTune/apple/$@ $^ -L../../lib/CLTune/apple -lcltune

clean:
	rm -f *.o
