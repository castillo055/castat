# build a program from 2 files and one shared header
CXX = g++
CXXFLAGS = -g -Wall -pedantic

castat: castat.o
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -vf castat castat.o
