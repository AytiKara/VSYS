all: myclient myserver probe

myserver: myserver.o FileController.o LdapAuth.o
		g++ myserver.o FileController.o LdapAuth.o -lldap -llber -o myserver

myserver.o: myserver.cpp
		g++ -c myserver.cpp

myclient: myclient.o FileController.o
		g++ myclient.o FileController.o -o myclient

myclient.o: myclient.cpp
		g++ -c myclient.cpp

probe: probe.o
		g++ probe.o -o probe -lldap -llber

probe.o: probe.cpp
		g++ -c probe.cpp

FileController.o: FileController.cpp FileController.h
		g++ -c FileController.cpp

LdapAuth.o: LdapAuth.cpp LdapAuth.h
		g++ -c LdapAuth.cpp 
clean:
		rm *.o
		rm myclient
		rm myserver
		rm FileController
		rm probe
		rm LdapAuth

release: clean
		tar -czvf myclient.tar.gz *