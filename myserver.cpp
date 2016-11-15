/* myserver.c */
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
#include <dirent.h>
#include <string>
#include <sstream>
#include "FileController.h"
#define BUF 1024
//#define PORT 6543
#define FBUF 500


using namespace std;
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
	int create_socket, new_socket;
	socklen_t addrlen;
	char buffer[BUF];
	int size;
	int PORT;
	if (argc < 3)
  	{
       printf("Usage: %s Port Downloadverzeichnis\n", argv[0]);
       exit(EXIT_FAILURE);
  	}

  	errno = 0;
  	PORT = strtol(argv[1], &argv[1], 10);

	struct sockaddr_in address, cliaddress;

	create_socket = socket (AF_INET, SOCK_STREAM, 0);

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons (PORT);

	if (bind ( create_socket, (struct sockaddr *) &address, sizeof (address)) != 0)
	{
		perror("bind error");
		return EXIT_FAILURE;
	}
	listen (create_socket, 1);

	addrlen = sizeof (struct sockaddr_in);

	pid_t pid ;

	while (1)
	{

		printf("Waiting for connections...\n");
		new_socket = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );

		pid = fork();
		if (pid != 0) {
			close(new_socket);
			continue;
		}
		if (new_socket > 0)
		{

			printf ("Client connected from %s:%d...\n", inet_ntoa (cliaddress.sin_addr), ntohs(cliaddress.sin_port));
			strcpy(buffer, "Welcome to myserver, Please enter your command:\n");
			send(new_socket, buffer, strlen(buffer), 0);
		}
		do
		{
			memset(buffer, 0, BUF);
			FileController fc;
			size = recvMsg(new_socket, buffer);

			if ( size > 0)
			{
				buffer[size] = '\0';
				string eingang = buffer;
				if (eingang == "put")
				{
					//Put empfangen
					memset(buffer, 0, BUF);

					//Dateiname empfangen
					char dateiname[BUF];
					memset(dateiname, 0, BUF);

					recvMsg(new_socket, dateiname);

					//Dateigröße empfangen
					char cSize[BUF];
					memset(cSize, 0, BUF);
					recvMsg(new_socket, cSize);
					long lsize = atol(cSize);
					memset(cSize, 0, BUF);

					int anz = 0;
					int pos = 0;

					string sDateiname = dateiname;
					sDateiname = "download/" + sDateiname;
					ofstream outfile (sDateiname.c_str(), ios::binary | ios::out);

					while (lsize)
					{

						if (lsize >= FBUF)
						{
							recvMsg(new_socket, buffer);
							lsize -= FBUF;
							outfile.write(buffer, FBUF);
						} else
						{
							recvMsg(new_socket, buffer);
							outfile.write(buffer, lsize);
							lsize = 0;

						}

						memset(buffer, 0, BUF);
						anz++;
					}
					outfile.close();
					memset(dateiname, 0, BUF);

					cout << " Fertig" << endl;

				}else if (eingang == "get")
				{

					char dateiname [BUF];
					memset(dateiname, 0, BUF);
					recvMsg(new_socket, dateiname);

					//Send der Dateigröße
					long lsize = fc.getSize(dateiname, 0);
					char  ssize[BUF] ;
					strcpy(ssize, numberToString(lsize).c_str());
					sendMsg(new_socket, ssize, strlen(ssize));

					char block[BUF];
					int anz = 0;


					string sDateiname = dateiname;
					sDateiname = "download/" + sDateiname;
					ifstream infile (sDateiname.c_str(), ios::binary |	ios::in);

					while (lsize)
					{

						if (lsize >= FBUF)
						{

							//Sendet den 500 byte großen Block
							infile.read(block, FBUF);
							sendMsg(new_socket, block, FBUF);
							//neue Position wird festgelegt
							lsize -= FBUF;

						}
						else
						{
							//Restlicher Block wird gelesen
							infile.read(block, lsize);

							//Letzter Block wird gesendet
							sendMsg(new_socket, block, lsize);
							lsize = 0;

						}

						memset(block, 0, BUF);
						anz++;
					}
					infile.close();
					cout << " Fertig" << endl;

				}else if(eingang=="list")
           		{
	           	  DIR *d;
	              struct dirent *dir;
	              //TODO: ändere download in eingabe von user
	              d = opendir(argv[2]);

	              string listOfFiles="";

	              FILE *pFile;
	              long size;
	              string path_filename= "";
	              stringstream ss;
	              string dumpstring = "";
				  char fileNames[BUF];
	              if(d)
	              {
	                while ((dir = readdir(d)) != NULL)
	                {
	                	//TODO ändere download in eingabe von user
	                	path_filename = "download/";
	                  	path_filename += dir->d_name;
	               	  	listOfFiles+=dir->d_name;
	                  	listOfFiles+="\n";
	                  	//file wird geöffnet
	                  	pFile = fopen (path_filename.c_str(),"rb");
	                  	if (pFile==NULL) perror ("Error opening file");
	                  	else
	                  	{
	                    	fseek (pFile, 0, SEEK_END);
	                    	size=ftell (pFile);
	                    	fclose (pFile);
	                    	//printf ("Size of : %ld bytes.\n",size);
	                    	dumpstring = "";
	                    	ss.str(string());
	                    	ss << size;
	                    	dumpstring = ss.str();
	                    	listOfFiles += dumpstring;
	                    	listOfFiles += " Bytes\n";
	                    	//cout<<dumpstring;
	                  	}
	                }
	                closedir(d);
	                strcat(fileNames, listOfFiles.c_str());
	              }
              	  sendMsg(new_socket, fileNames, strlen(fileNames));


            }
            else
           	cout<<"Message received: "<<eingang<<endl;

			}
			else if (size == 0)
			{
				printf("Client closed remote socket\n");
				break;
			}
			else
			{
				perror("recv error");
				return EXIT_FAILURE;
			}
		}
		while (strncmp (buffer, "quit", 4)  != 0);
		close (new_socket);
	}
	close (create_socket);
	return EXIT_SUCCESS;
}
