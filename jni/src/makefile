# export JAVA_HOME="/Library/Java/JavaVirtualMachines/jdk1.8.0_31.jdk/Contents/Home/include -I/Library/Java/JavaVirtualMachines/jdk1.8.0_31.jdk/Contents/Home/"
# g++ -O3 -std=c++11 -I../include -I$JAVA_HOME/include -I/include/darwin -c -o JTuner.o JTuner.cpp
# make
# cp libjtuner.dylib /Users/stevenstewart/git/gpuclu/lib/JTuner/apple/

CXX := g++
LIBRARY := libjcltune.dylib
OBJFILES := JTuner.o
INCLUDES := -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin -I../include
CXXFLAGS := $(INCLUDES) -std=c++11

$(LIBRARY): $(OBJFILES)
	$(CXX) -shared -o ../../lib/JCLTune/apple/$@ $^ -L../../lib/CLTune/apple -lcltune

clean:
	rm -f *.o

#g++ -O3 -std=c++11 -I../include -I$JAVA_HOME/include -I$JAVA_HOME/include/darwin -c -o JTuner.o JTuner.cpp
#g++ -shared -o libjcltune.dylib JTuner.o -L../../lib/JCLTune/apple/ -lcltune
#mv libjcltune.dylib ../../lib/JCLTune/apple/
