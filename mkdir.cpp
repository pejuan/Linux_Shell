#include <iostream>
#include <sys/stat.h>

using namespace std;


int main(int argc, char const *argv[])
{
	//crea un directorio con ruta absoluta o relativa
	//string dirName = ingreso.substr(cont+1,ingreso.size());
	//char fileName[100];
	/////////////cin >> dirName;
	//stringstream ss;
	//ss << currentDirectory << dirName;
	//ss >> fileName;
	//mkdir(*argv , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (mkdir(argv[0], S_IRUSR | S_IWUSR | S_IXUSR) == 0){
		return 0;
	}
	//cout << "hello world" << endl;
	else{
		return -1;
	}
}