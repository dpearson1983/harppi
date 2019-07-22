CXX=g++ -std=c++14
CXXFLAGS= -march=native -mtune=native -O3
DEPS=harppi.hpp

harppimake: harppi.cpp
	$(CXX) $(CXXFLAGS) -c -o libharppi.a harppi.cpp
	
localinstall: harppi.cpp
	mkdir -p $(HOME)/include
	mkdir -p $(HOME)/lib
	cp harppi.hpp $(HOME)/include/
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $(HOME)/lib/libharppi.so harppi.cpp
	
globalinstall: harppi.cpp
	cp harrpi.hpp /usr/include
	mkdir -p /usr/lib/harppi
	$(CXX) $(CXXFLAGS) -c -o /usr/lib/harppi/harppi.a harppi.cpp
	cd /usr/lib
	ls -l harppi/harppi.a
	cd -
