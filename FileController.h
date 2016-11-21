#include <iostream>
using namespace std;

class FileController
{
private:
	

public:
	FileController();
	~FileController();
	/*typeOfSide :
					0 is Server
					1 is Client
	*/
	void writeFile(char filename[], string datei, int typeOfSide);
	string getFile(char filename[], int typeOfSide );
	bool checkIfExists(string filename, int typeOfSide);
	long getSize(char filename[],int typeOfSide);

};
