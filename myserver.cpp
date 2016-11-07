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
#include "FileController.h"
#define BUF 1024
#define PORT 6543
#define FBUF 500


using namespace std;


int main (void)
{
	int create_socket, new_socket;
	socklen_t addrlen;
	char buffer[BUF];
	int size;
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

	while (1)
	{
		printf("Waiting for connections...\n");
		new_socket = accept ( create_socket, (struct sockaddr *) &cliaddress, &addrlen );
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
			size = recv (new_socket, buffer, BUF  , 0);														//<<-------

			if ( size > 0)
			{

				// 	if (size > 1)
				// 	{
				buffer[size] = '\0';
				string eingang = buffer;
				if (eingang == "put")
				{

					//Put empfangen
					cout << ">> " << buffer << " Empfangen" << endl;
					memset(buffer, 0, BUF);

					//Dateiname empfangen
					char dateiname[BUF];
					memset(dateiname, 0, BUF);

					size=recv(new_socket, dateiname, BUF-1 , 0);												//<<-------
					dateiname[size]='\0';
					cout << ">> " << dateiname << " Empfangen" << endl;

					//Dateigröße empfangen
					char cSize[BUF];
					memset(cSize, 0, BUF);

					size= recv(new_socket, cSize, BUF - 1 , 0);													//<<-------
					cSize[size]='\0';
					cout << ">> Size: " << cSize << " bytes " << "Empfangen" << endl;
					
					long lsize = atol(cSize);
					cout << ">> Long Size " << lsize << endl;
					memset(cSize, 0, BUF);


					int anz = 0;
					cout << "!!!!!!!Achtung Schleife!!!!!!" << endl;
					char datei[lsize];

					int pos = 0;
					char antwort[]=" OK Transfer! ";
					
					
					while (lsize)
					{
						if (lsize >= FBUF)
						{
							recv(create_socket, buffer, BUF - 1, 0);
							cout << "Block empfangen" << endl;
							
							for (int k = 0; k < FBUF; k++)
								datei[pos + k] = buffer[k];


							lsize -= FBUF;
							pos += FBUF;

							send(new_socket,antwort,strlen(antwort),0);
					

						} else {
							recv(create_socket, buffer, BUF-1, 0);
							cout << "Last block lsize: " << lsize << endl;
							cout<<buffer<<endl;
							for (int k = 0; k < lsize; k++)
								datei[pos + k] = buffer[k];


							pos+=lsize;
							lsize = 0;
							cout << "Letzten Block empfangen size: "<<pos << endl;
							send(new_socket,antwort,strlen(antwort),0);
					
							break;
						}
						memset(buffer, 0, BUF);
						cout << "Block " << anz << " Empfangen Size: " << pos << "/"<<lsize<<endl;
						anz++;
					}
					cout<<datei<<endl;
					fc.writeFile(dateiname, datei, lsize, 0);
					memset(dateiname, 0, BUF);
					memset(datei, 0, lsize);
					cout << "Fertig" << endl;



					// 			// recv(new_socket,buffer,BUF-1,0);
					// 			// char dateiname[BUF];
					// 			// strcpy(dateiname,buffer);
					// 			// memset(buffer,0,BUF);
					// 			// recv(new_socket,buffer,BUF-1,0);

					// 			// long lsize= atol(buffer);

					// 			// char datei[lsize];
					// 			// memset(buffer,0,BUF);
					// 			// for(int i=0;i<lsize;i++)
					// 			// {
					// 			// 	recv(new_socket,buffer,BUF-1,0);
					// 			// 	for(int k = 0; k=sizeof(buffer);k++)
					// 			// 	{
					// 			// 		datei[i]=buffer[k];
					// 			// 		i+=k;
					// 			// 	}
					// 			// 	memset(buffer,0,BUF);
					// 			// }
					// 			// cout<<"Fertig"<<endl;

					// 			// fc.writeFile(dateiname ,datei,lsize,0);


					// 			// memset(buffer, 0, BUF);
					// 			// recv(new_socket, buffer, BUF-1,0);
					// 			// char filename[BUF];
					// 			// strcpy(filename,buffer);

					// 			// memset(buffer, 0, BUF);
					// 			// cout<<filename<<endl;
					// 			// if(filename!="NO"){
					// 			// recv(new_socket, buffer, BUF-1, 0);
					// 			// buffer;

					// 			//  cout<<"Filename= "<<filename<<endl;
					// 			//  cout<<"Datei= "<<buffer;


					// 			// 	fc.writeFile(filename, buffer,0);

					// 			// }else
					// 			// cout<<"Geht nicht!"<<endl;

				}
				// 		else if (eingang == "get")
				// 		{

				// 			// 	memset(buffer, 0, BUF);
				// 			//   	recv(new_socket, buffer, BUF-1,0);
				// 			//   	string filename = buffer;
				// 			//   	cout<<filename<<endl;
				// 			//  if(!fc.checkIfExists(filename,0)){
				// 			//    filename = "NO";
				// 			//    cout<<"Die Datei wurde nicht gefunden."<<endl;
				// 			//  }else{
				// 			//  	send(new_socket,filename.c_str(),filename.size(),0);
				// 			//    filename = fc.getFile(filename,0);
				// 			//    cout<<filename<<endl;
				// 			//  }
				// 			// send(new_socket, filename.c_str(),filename.size(), 0);

				// 		}
				// 		else if (eingang == "list")
				// 		{

				// 			DIR *d;
				// 			struct dirent *dir;
				// 			d = opendir("./download");

				// 			int counterStrike = 0;
				// 			string gPaket = "";
				// 			if (d)
				// 			{
				// 				while ((dir = readdir(d)) != NULL)
				// 				{

				// 					gPaket += dir->d_name;
				// 					gPaket += "\n";
				// 				}
				// 				closedir(d);

				// 			}
				// 			send(new_socket, gPaket.c_str(), gPaket.size(), 0);

				// 		}
				// 		else {
				// 			cout << "Message received: " << buffer << endl;
				// 		}
				//printf ("Message received: %s\n", buffer);
				// } else
				// {
				// 	int a = buffer[0];
				// 	cout << "Size 1 buffer Ausgabe --> " << a << endl;
				// }
				// else
				// 	cout<<"Message: "<<buffer<<endl;
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
