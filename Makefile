all: myclient myserver probe

myserver: myserver.o FileController.o
		g++ myserver.o FileController.o -o myserver

myserver.o: myserver.cpp
		g++ -c myserver.cpp

myclient: myclient.o FileController.o
		g++ myclient.o FileController.o -o myclient

myclient.o: myclient.cpp
		g++ -c myclient.cpp

probe: probe.o
		g++ probe.o -o probe

probe.o: probe.cpp
		g++ -c probe.cpp

FileController.o: FileController.cpp FileController.h
		g++ -c FileController.cpp
clean:
		rm *.o
		rm myclient
		rm myserver
		rm FileController

release: clean
		tar -czvf myclient.tar.gz *
