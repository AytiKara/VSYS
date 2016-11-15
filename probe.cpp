#include <iostream>
#include <fstream>
#include <dirent.h>
using namespace std;

void getFile(string filename)
{
	string line;
	//char * abc;
	ifstream myfile(filename.c_str(), ios::binary);

	if (myfile.is_open())
	{
		while ( getline (myfile, line) )
		{
			cout << line << endl;
		}
		myfile.close();

	}
}
void writeFile()
{
	ofstream myfile;
	myfile.open ("bild.txt");
	for (int i = 0; i < 1000; i++)
		myfile << i << "\n";
	myfile.close();
}
long getFileBinary(char *memblock)
{
	ifstream infile ("bild.txt", ios::binary |	ios::in);
	infile.seekg (0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);

	infile.read(memblock, size);

	infile.close();

	return size;

}

void setFileBinary(char *memblock, long size)
{

	ofstream outfile ("bild2.txt", ios::binary |	ios::out);
	cout << "Hey 8" << endl;

	outfile.write(memblock, size);
	cout << "Hey 9" << endl;

	outfile.close();
}


void newDataFile()
{
	std::ifstream infile ("bild.txt", ios::binary |	ios::in);
	std::ofstream outfile ("bild2.txt", ios::binary |	ios::out);
	// get size of file
	infile.seekg (0, infile.end);
	long size = infile.tellg();
	infile.seekg (0);
	// allocate memory for file content
	char *buffer = new char(size);
	// read content of infile
	infile.read (buffer, size);
	// write to outfile
	outfile.write (buffer, size);

// 	// release dynamically-allocated memory
	delete []	buffer;
	cout << "Hey 6" << endl;
	infile.close();
	outfile.close();

}



int main()
{





// char * mblock;
// streampos size=getFileBinary(mblock);
// setFileBinary(mblock,size);


// cout << "Hey 01" << endl;

char  memblock[500];
long size = getFileBinary(memblock);
cout << "Hey 02: " << size << endl;

setFileBinary(memblock, size);
cout << "Hey 03" << endl;


// cout << "Hey 04" << endl;
//writeFile();

// newDataFile();
//delete [] mblock;
//getFile("abc.txt");

}
