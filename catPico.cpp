#include <iostream>
#include <stdio.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <fnmatch.h>
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char const *argv[])
{
	string argumento = argv[0];
	vector<string> ruta;
	vector<string> archivos;
	char* duplicado = strdup(argumento.c_str());
	char* token = strtok(duplicado, "¿");
	while(token != NULL){
		ruta.push_back(string(token));
		// the call is treated as a subsequent calls to strtok:
		// the function continues from where it left in previous invocation
		token = strtok(NULL, "¿");
	}
	string Arch = ruta[1];
	char* duplicado2 = strdup(Arch.c_str());
	char* token2 = strtok(duplicado2, ">");
	while(token2 != NULL){
		archivos.push_back(ruta[0]+string(token2));
		// the call is treated as a subsequent calls to strtok:
		// the function continues from where it left in previous invocation
		token2 = strtok(NULL, ">");
	}


	//char origen[200] = archivos[0].c_str();
	//char destino[200] = archivos[1].c_str();

	string lineOrigin;
	string originContent = "";
	ifstream archivoOrin(archivos[0].c_str());
	if(archivoOrin.is_open()){
		while(getline(archivoOrin,lineOrigin)){
		originContent += lineOrigin += '\n';
		}
	}

	ofstream myFileN(archivos[1].c_str(), ios::out);
	if(myFileN.is_open()){
		myFileN << originContent << '\n';
		myFileN.close();
	}

	originContent = "";

	return 0;
}