#include <iostream>
using namespace std;

class FileController
{
private:
	string filename;

public:
	FileController(string pFilename);
	~FileController();

	/*typeOfSide :
					0 is Server
					1 is Client
	*/
	void writeFile(string datei);
	string getFile();
	bool checkIfExists(string filename, int typeOfSide);
	long getSize(char filename[],int typeOfSide);
	bool checkUserIp(string uIp);

};
