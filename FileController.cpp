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
void FileController::writeFile(char filename[], char  *datei, long size, int typeOfSide)
{

	string eingabe = filename;

	if (!typeOfSide)
		eingabe = "download/" + eingabe;


	ofstream outfile (eingabe.c_str(), ios::binary | ios::out); 
	outfile.write(datei, size);
	outfile.close();

}

void FileController::getFile(char filename[], char * datei, long size, int typeOfSide)
{
	string eingabe = filename;

	if (!typeOfSide)
		eingabe = "download/" + eingabe;

	ifstream infile (eingabe.c_str(), ios::binary |	ios::in); 
	infile.read(datei, size);
	infile.close();



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

long FileController::getSize(char filename[])
{
	ifstream infile (filename, ios::binary |	ios::in);
	infile.seekg (0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	infile.close();
	return size;
}