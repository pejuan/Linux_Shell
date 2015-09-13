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
#include <dirent.h>
#include <errno.h>
#include <sstream>
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
	string ingreso;
	while(true){
		if (verificacion){
			cout<<"mi_sh@"<<currentDirectory<<">";
		}else{
			
		}
		
		getline(cin,ingreso);
		if (find(listaComandos.begin(),listaComandos.end(),ingreso) != listaComandos.end()){
			/* code */
			if (ingreso=="mkdir"){
				
			}else if(ingreso=="cd"){
				string nuevo;
				cin>>nuevo;
				currentDirectory += (nuevo+"/");
				verificacion = false;

			}else if(ingreso=="clear"){
				for (int i = 0; i < 25; i++){
					cout<<endl;
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