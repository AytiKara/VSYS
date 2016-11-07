/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "FileController.h"
#include <sstream>
#define BUF 1024
#define PORT 6543
#define OK 1
#define NOTOK 0
#define FBUF 500

using namespace std;



int main (int argc, char **argv)
{
	int create_socket;
	char buffer[BUF];
	struct sockaddr_in address;
	int size;

	if ( argc < 2 )
	{
		printf("Usage: %s ServerAdresse\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket error");
		return EXIT_FAILURE;
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons (PORT);
	inet_aton (argv[1], &address.sin_addr);

	if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
	{
		printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
		size = recv(create_socket, buffer, BUF - 1, 0);
		if (size > 0)
		{
			buffer[size] = '\0';

			printf("%s", buffer);
			memset(buffer, 0, sizeof(buffer));
		}
	}
	else
	{
		perror("Connect error - no server available");
		return EXIT_FAILURE;
	}
	char eingabe[BUF];
	do
	{
		memset(eingabe, 0, BUF);
		cout << "Anfrage: ";

		cin >> eingabe;
		// transform(eingabe.begin(),eingabe.end(),eingabe.begin(),::tolower);

		FileController fc;
		send(create_socket, eingabe, strlen(eingabe), 0);
		cout << ">> Sende: " << eingabe << endl;
		string ee = eingabe;
		int zz = 0;
		if (ee == "put")
		{

			char dateiname [BUF];
			memset(dateiname, 0, BUF);

			//Eingabe vom Dateinamen
			cout << "Datei: ";
			cin >> dateiname;

			//sende Dateinamen
			send(create_socket, dateiname, strlen(dateiname), 0);
			cout << ">> Sende Dateiname: " << dateiname << endl;

			//Ermittle Size von Datei
			long lsize = fc.getSize(dateiname);
			cout << ">> Size ermittelt von " << dateiname << " ermittelt: " << lsize << " bytes" << endl;


			//Datei lesen und speichern
			char datei[lsize];
			cout << ">> File lesen" << endl;
			fc.getFile(dateiname, datei, lsize, 1);
			fc.writeFile(dateiname,datei,lsize,1);
			cout << ">> Datei gelesen und Temporär gespeichert" << endl;


			//Sende Dateigröße
			stringstream ss;
			string ssize;
			ss << lsize;
			ss >> ssize;
			send(create_socket, ssize.c_str(), ssize.size(), 0);
			cout << ">> Sende Size: " << ssize << " bytes" << endl;



			int pos = 0;
			char block[BUF];
			int anz = 0;

			char ausgabe[BUF];

cout<<datei<<endl;
			cout<<"!!!!!!Achtung Schleife!!!!!!"<<endl;
			while (lsize)
			{

				size = recv(create_socket, ausgabe, BUF-1, 0);
				ausgabe[size] = '\0';
				cout << "<<<<<<<<<" << ausgabe << endl;
				memset(ausgabe, 0, BUF);

				if (lsize >= FBUF)
				{

					//Blockweise sende 1 Block ist 500 Byte groß
					for (int k = pos; k < FBUF; k++){
						block[k] = datei[k];
						cout<<"#";
					}

					send(create_socket, block, BUF, 0);
					cout << "Sende Block " << anz << " lsize: " << lsize << endl;

					lsize -= FBUF;
					pos += FBUF;


				}
				else
				{

					cout << "Last block lsize: " << lsize << endl;
					for (int k = pos; k <= lsize; k++){
						block[k] = datei[k];
						cout<<"#";
					}

					send(create_socket, block, BUF, 0);

					cout << "Sende Letzten Block" << endl;
					lsize = 0;


				}
				cout << "Rest lsize: " << lsize << endl;
				memset(block, 0, BUF);
				anz++;
			}
			cout << ">> Fertig" << endl;



			// 	// send(create_socket, eingabe, BUF,0);
			// 	// cout<<"Datei: ";
			// 	//   	memset(eingabe, 0 ,BUF);
			// 	//   	cin>>eingabe;
			// 	// 	send(create_socket, eingabe,BUF, 0);
			// 	//   	if(!fc.checkIfExists(eingabe,1)){
			// 	//     	strcpy(eingabe,"NO");
			// 	//     	cout<<"Die Datei wurde nicht gefunden."<<endl;
			// 	//   	}else{
			// 	//     	strcpy(eingabe,fc.getFile(eingabe,1));
			// 	//   }
			// 	// send(create_socket, eingabe,BUF, 0);

		}
		// else if (eingabe == "get")
		// {


		// 	//        	cout<<"Datei: ";
		// 	//        	cin>>eingabe;
		// 	// send(create_socket, eingabe, BUF,0);


		// 	//        	memset(buffer, 0, BUF);
		// 	//         	recv(create_socket, buffer, BUF-1,0);
		// 	//         	char filename[BUF] = buffer;

		// 	//          memset(buffer, 0, BUF);
		// 	//         	if(filename!="NO"){

		// 	//          recv(create_socket, buffer, BUF-1, 0);
		// 	//         	string datei = buffer;

		// 	//          	fc.writeFile(filename, datei,1);
		// 	//          }else
		// 	//          cout<<"Die Datei wurde nicht gefunden"<<endl;

		// }
		// else if (eingabe == "list")
		// {
		// 	char irg[BUF];
		// 	memset(irg, 0, BUF);

		// 	size = recv(create_socket, irg, BUF - 1, 0);
		// 	cout << irg << endl;
		// }

	}
	while (eingabe != "quit");
	close (create_socket);
	return EXIT_SUCCESS;
}
