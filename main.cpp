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

using namespace std;


int getdir (string dir, vector<string> &files){
	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL){
		cout << "Error(" << errno << ") opening " << dir << endl;
		return errno;
	}
	while ((dirp = readdir(dp)) != NULL){
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
bool contains(char* comando, vector<string> listaComandos){
	for (int i = 0; i < listaComandos.size(); ++i){
		if (!strcmp(comando,listaComandos[i].c_str())){
			return true;
		}

	}
	return false;
}
bool containsStr(string comando, vector<string> listaComandos){
	for (int i = 0; i < listaComandos.size(); ++i){
		if (comando==listaComandos[i]){
			return true;
		}

	}
	return false;
}
int main(int argc, char const *argv[]){
	
	vector<string> listaComandos;
	bool verificacion = true;
	string currentDirectory = "/home/";
	listaComandos.push_back("mkdir");
	listaComandos.push_back("cd");
	listaComandos.push_back("ls");
	listaComandos.push_back("clear");
	listaComandos.push_back("exit");
	listaComandos.push_back("cat");
	listaComandos.push_back("chmod");
	string ingreso;
	int cont;
	string substring = "nada";
	while(true){
		if (verificacion){
			cout<<"mi_sh@"<<currentDirectory<<">";
		}else{
			
		}
		
		if (verificacion){
			/* code */
			getline(cin,ingreso);
			cont = 0;
			for (int i = 0; i < ingreso.size(); i++){
				if (ingreso[i] == ' '){
					break;
				}else{
					cont++;
				}
			}
			substring = ingreso.substr(0,cont);
		}
		
		//cout<<substring<<endl;
		
		//cin>>ingreso;
		/*char* wholeCommand = new char[ingreso.length()+1];
		strcpy (wholeCommand, ingreso.c_str());
		char * pch;
		pch = strtok (wholeCommand," ");
		//cout<<wholeCommand<<endl;
		cout<<pch<<endl;*/

		if (containsStr(substring,listaComandos) && verificacion){
			/* code */
			if (ingreso=="mkdir"){
				string dirName;
				char fileName[100];
				cin >> dirName;
				stringstream ss;
				ss << currentDirectory << dirName;
				ss >> fileName;
			    mkdir(fileName , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			    verificacion = false;

			}else if(ingreso=="chmod"){
				char nombreArchivo[300];
				cin >> nombreArchivo;
				cout << "Permissions: ";
				string permissions;
				cin >> permissions;		
				ofstream permissionsFile;
				permissionsFile.open ("permissions.txt",std::ios_base::app);
				permissionsFile << nombreArchivo << ',' << permissions << '\n';
				permissionsFile.close();
				verificacion = false;

			}else if(substring=="cd"){
				string carpeta = ingreso.substr(cont+1,ingreso.size());
				//cout<<substring2<<endl;
				//string nuevo;
				//cin>>nuevo;
				currentDirectory += (carpeta+"/");
				verificacion = false;

			}else if(ingreso=="clear"){
				for (int i = 0; i < 25; i++){
					cout<<endl<<endl;
				}
			}else if(ingreso=="ls"){
				string dir = currentDirectory;
				vector<string> files = vector<string>();
				getdir(dir,files);
				for (unsigned int i = 0;i < files.size();i++)
				{
				cout << files[i] << endl;
				}
			}else if(ingreso=="cat"){
				char nombreArchivo[300];
				cin >> nombreArchivo;
				stringstream ss;
				ss<<currentDirectory<<nombreArchivo;
				ss>>nombreArchivo;
				string concatName;

				string lineV;
				ifstream archivoV(nombreArchivo, ios::in);
				//archivo.open();
				if (archivoV.is_open()){
					while(getline(archivoV,lineV)){
						//tokenize here
					}
					archivoV.close();
				}


				/*Example: rwx
				if string[0] == r then continue executing cat, else cout << "No tiene permiso de lectura"
				*/
				string line;
				ifstream archivo(nombreArchivo, ios::in);
				//archivo.open();
				if (archivo.is_open()){
					while(getline(archivo,line)){
						cout<<line<<endl;
					}
					archivo.close();
				}
				verificacion = false;
			}else if(ingreso=="exit"){
				break;
			}
		}else{
			if (verificacion)
			{
				/* code */
				cout<<"Comando no existe."<<endl;

			}else{
				verificacion = true;
			}
			
		}

	}


	return 0;
}	