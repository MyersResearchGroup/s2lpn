default: ../../bin/s2lpn

../../bin/s2lpn: load.o s2lpn.o
	g++ -Wall load.o s2lpn.o -o s2lpn
	mv s2lpn ../../bin

load.o:	load.c load.h
	g++ -Wall -c load.c

s2lpn.o:	s2lpn.c load.h
	g++ -Wall -c s2lpn.c

clean:
	rm -f *.o ../../bin/s2lpn
