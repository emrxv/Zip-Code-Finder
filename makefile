all: zipcode fed2cs2303

zipcode: zipcode.o zipfed.o
	g++ -g zipcode.o zipfed.o -o zipcode

zipcode.o: zipcode.c zipfed.hpp
	g++ -g -c zipcode.c

fed2cs2303: fed2cs2303.o zipfed.o
	g++ -g fed2cs2303.o zipfed.o -o fed2cs2303

fed2cs2303.o: fed2cs2303.c zipfed.hpp
	g++ -g -c fed2cs2303.c

zipfed.o: zipfed.cpp zipfed.hpp
	g++ -g -c zipfed.cpp

zipfilter.o: zipfed.hpp
	g++ -g -c zipfilter.c

docs:
	doxygen
	cp -rp html/* ~/public_html/cs2303_hw06

clean:
	rm -r *.o fed2cs2303 zipcode 