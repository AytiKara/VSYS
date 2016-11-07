#include <iostream>
using namespace std;

class FileController
{
public:
	FileController();
	~FileController();
	/*typeOfSide :
					0 is Server
					1 is Client
	*/
	void writeFile(char filename[], char *dateiname, long size, int typeOfSide);
	void getFile(char filename[], char * datei, long size, int typeOfSide );
	bool checkIfExists(string filename, int typeOfSide);
	long getSize(char filename[],int typeOfSide);

};
