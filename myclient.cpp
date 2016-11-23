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
//#define PORT 6543
#define FBUF 500
#define PERCENTAGE_SCALE 75

using namespace std;
void showProgressBar(double chunksNr, double chunksTotal) {
    int percentageChunksSent = (int) (chunksNr/chunksTotal * PERCENTAGE_SCALE);
    printf("[");
    for(int i = 0; i < percentageChunksSent; i++) printf("#");
    for(int i = percentageChunksSent; i < PERCENTAGE_SCALE; i++) printf(" ");
    printf("] %d%s", percentageChunksSent*100/PERCENTAGE_SCALE, "%");
    fflush(stdout);
    printf("\r");
}

template <typename T>
string numberToString(T x)
{
	stringstream ss;
	string ssize;
	ss << x;
	ss >> ssize;
	return ssize;
}


void sendMsg(int create_socket, char * input, int msgSize)
{
	string ssize = numberToString(msgSize);
	string number = "";
	if (msgSize < 100) {
		number += "0";
		if (msgSize < 10)
			number += "0";
	}
	number += ssize;
	send(create_socket, ssize.c_str(), 3, 0);
	send(create_socket, input, msgSize , 0);

}


int recvMsg(int new_socket, char * input)
{
	memset(input, 0, BUF);
	if (recv(new_socket, input, 3, 0) > 0) {
		string sSize = input;
		int iSize = atoi(sSize.c_str());

		memset(input, 0, 4);
		int ii = recv(new_socket, input, iSize, 0);
		return ii;
	} else
		return 0;

	return -1;
}

int main (int argc, char **argv)
{
	int create_socket;
	char buffer[BUF];
	struct sockaddr_in address;
	int size;
	int port;

	if ( argc < 3 )
	{
		printf("Usage: %s ServerAdresse Port\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket error");
		return EXIT_FAILURE;
	}

	port = atol(argv[2]);

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons (port);
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

	bool userOk = false;
	int userEntryCount = 0;
	do
	{
		// do
		// {
		// 	cout<<"Username: "<<endl;


		// }while();
		memset(buffer,0,BUF);
		recvMsg(create_socket, buffer);
		string recvMessage = buffer;
		if(recvMessage == "ne")
		{
			cout<<"Sie sind gesperrt!"<<endl;
			break;
		}

		while (userEntryCount < 3 && !userOk) {
			userEntryCount++;
			//Sende User id
			memset(eingabe, 0, BUF);
			cout << "Uid: "; cin >> eingabe;
			sendMsg(create_socket, eingabe, strlen(eingabe));

			//Sende Passwort
			memset(eingabe, 0, BUF);
			strcat(eingabe, getpass("Passwort: "));
			sendMsg(create_socket, eingabe, strlen(eingabe));

			recvMsg(create_socket, buffer);
			string sok = buffer;
			if (sok == "ok") {
				userOk = true;
				cout << "Anmeldung erfolgreich" << endl;
				break;
			}
			else
			{
				cout << buffer << endl;
			}

		}

		if (!userOk)break;




		memset(eingabe, 0, BUF);
		cout << "Anfrage: ";	cin >> eingabe;
		//File Objekt ist zuständig für das schreiben und lesen der Datei

		sendMsg(create_socket, eingabe, strlen(eingabe));
		string ee = eingabe;

		if (ee == "put")
		{

			char dateiname [BUF];
			memset(dateiname, 0, BUF);

			//Eingabe und Senden --> Dateinamen --> Server
			cout << "Datei: ";
			cin >> dateiname;
			sendMsg(create_socket, dateiname, strlen(dateiname));
			FileController fc(dateiname);
			//Send der Dateigröße
			long lsize = fc.getSize(dateiname, 1);
			char  ssize[BUF] ;
			strcpy(ssize, numberToString(lsize).c_str());
			sendMsg(create_socket, ssize, strlen(ssize));

			char block[BUF];
			int anz = 0;

			ifstream infile (dateiname, ios::binary |	ios::in);
			int pos=0;
			int totalPacket = lsize /FBUF;

			while (lsize)
			{

				if (lsize >= FBUF)
				{

					//Sendet den 500 byte großen Block
					infile.read(block, FBUF);
					sendMsg(create_socket, block, FBUF);


					//neue Position wird festgelegt
					lsize -= FBUF;
					pos++;

				}
				else
				{
					//Restlicher Block wird gelesen
					infile.read(block, lsize);

					//Letzter Block wird gesendet
					sendMsg(create_socket, block, lsize);
					lsize = 0;
					pos++;
				}
				showProgressBar(pos,totalPacket);
				// Soll warten bevor er das nächste Paket schickt, 
				// manchmal schickt der Client zu früh los bevor er was ausgeben kann
				 usleep(1);
				memset(block, 0, BUF);
				anz++;
			}
			cout << endl;
			infile.close();
			pos--;
			cout << "Fertig: "<<pos<<"/"<<totalPacket<<" Pakete abgeschickt" << endl;

		}
		else if (ee == "get")
		{


			//Dateiname empfangen
			char dateiname[BUF];
			memset(dateiname, 0, BUF);
			cout << "Datei: "; cin >> dateiname;
			sendMsg(create_socket, dateiname, strlen(dateiname));

			//Dateigröße empfangen
			char cSize[BUF];
			memset(cSize, 0, BUF);
			recvMsg(create_socket, cSize);
			long lsize = atol(cSize);
			memset(cSize, 0, BUF);

			int anz = 0;
			int pos = 0;

			ofstream outfile (dateiname, ios::binary | ios::out);
			int totalPacket= lsize / FBUF;

			while (lsize)
			{

				if (lsize >= FBUF)
				{
					recvMsg(create_socket, buffer);
					lsize -= FBUF;
					outfile.write(buffer, FBUF);
					pos++;
				} else
				{
					recvMsg(create_socket, buffer);
					outfile.write(buffer, lsize);
					lsize = 0;
					pos++;
				}
				showProgressBar(pos,totalPacket);
				// Soll warten bevor er das nächste Paket schickt, 
				// manchmal schickt der Client zu früh los bevor er was ausgeben kann
				 usleep(1);
				memset(buffer, 0, BUF);
				anz++;
			}
			cout<<endl;
			outfile.close();
			memset(dateiname, 0, BUF);
			pos--;
			cout << "Fertig "<<pos<<"/"<<totalPacket<<" bekommen" << endl;

		} else if (ee == "list")
		{
			char list[BUF];
			memset(list, 0, BUF);
			recvMsg(create_socket, list);
			cout << list << endl;
		}
	}
	while (strcmp(eingabe, "quit"));
	close (create_socket);
	return EXIT_SUCCESS;
}
