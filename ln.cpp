#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;


int main(int argc, char const *argv[])
{
	string argumento = argv[0];
	vector<string> ruta;
    vector<string> archivos;

    char* duplicado2 = strdup(argumento.c_str());
    char* token2 = strtok(duplicado2, " ");
    while(token2 != NULL){
        archivos.push_back(string(token2));
        // the call is treated as a subsequent calls to strtok:
        // the function continues from where it left in previous invocation
        token2 = strtok(NULL, " ");
    }

    string archivo1 = archivos[0];
    string archivo2 = archivos[1];

    if (symlink(archivo1.c_str(), archivo2.c_str()) != -1 )
    {
    	
    }
    else{
    	cout << "Error al ejecutar ln" << endl;
    }

	return 0;
}