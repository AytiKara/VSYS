#include <iostream>
#include <fstream>
#include <string.h>
#include "FileController.h"

using namespace std;
FileController::FileController()
{

}
FileController::~FileController()
{

}
void FileController::writeFile(char filename[],string datei, int typeOfSide)
{

	string eingabe = filename;

	if (!typeOfSide)
		eingabe = "download/" + eingabe;


	ofstream myfile;
	myfile.open (eingabe.c_str());
	myfile << datei;
	myfile.close();

}

string FileController::getFile(char filename[], int typeOfSide)
{
	string eingabe = filename;
	string zeile;
	if (!typeOfSide)
		eingabe = "download/" + eingabe;

	string line;
	ifstream myfile (eingabe.c_str());
	if (myfile.is_open())
	{
		while ( getline (myfile, line) )
		{
			zeile += line+"\n";
			
		}
		myfile.close();
	}

	return zeile;

}

bool FileController::checkIfExists(string filename, int typeOfSide)
{

	if (!typeOfSide)
		filename = "download/" + filename;

	ifstream myfile(filename.c_str());
	if (myfile.is_open())
	{
		return true;
	}
	else return false;
}

long FileController::getSize(char filename[], int typeOfSide)
{
	string eingabe = filename;

	if (!typeOfSide)
		eingabe = "download/" + eingabe;

	ifstream infile (eingabe.c_str(), ios::binary |	ios::in);
	infile.seekg (0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	infile.close();
	return size;
}