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
int filter(const struct dirent *dir){
	uid_t user;
	struct stat dirinfo;
	int len = strlen(dir->d_name) + 7; 
	char path[len];

	strcpy(path, "/proc/");
	strcat(path, dir->d_name);
	user = getuid();
	if (stat(path, &dirinfo) < 0) {
	perror("processdir() ==> stat()");
	exit(EXIT_FAILURE);
	}
	return !fnmatch("[1-9]*", dir->d_name, 0) && user == dirinfo.st_uid;
}

void processdir(const struct dirent *dir){
     puts(dir->d_name);
}
int main(int argc, char const *argv[]){

	pid_t pid;

	int fd[2];
	pipe(fd);
	
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
	listaComandos.push_back("rmdir");
	listaComandos.push_back("rm");
	listaComandos.push_back("ln");
	listaComandos.push_back("ps");
	listaComandos.push_back("uname");
	listaComandos.push_back("kill");
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

		if (containsStr(substring,listaComandos) && verificacion){
			/* code */
			if (substring=="mkdir"){
				//crea un directorio con ruta absoluta o relativa
				string dirName = ingreso.substr(cont+1,ingreso.size());
				char fileName[100];
				/////////////cin >> dirName;
				stringstream ss;
				ss << currentDirectory << dirName;
				ss >> fileName;
			    mkdir(fileName , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			    verificacion = false;

			}else if(ingreso=="chmod"){
				//cambia el acceso a un archivo específico.
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
			}else if(ingreso=="cd"){
				//cambiar de directorio
				currentDirectory = "/home/";
			}else if(substring=="cd"){
				string carpeta = ingreso.substr(cont+1,ingreso.size());
				if (carpeta == ".."){
					int counterslash = 0;
					for (int i = 0; i < currentDirectory.size(); ++i){
						if(currentDirectory[i]=='/')
							counterslash++;
					}
					int counterletter = 0;
					int counterslash2 = 0;
					for (int i = 0; i < currentDirectory.size(); ++i){
						if ((counterslash-1)==counterslash2){
							break;
						}else if(currentDirectory[i]=='/'){
							counterslash2++;
						}
						counterletter++;

					}
					string substri = currentDirectory.substr(0,counterletter);
					currentDirectory = substri;

				}else{
					currentDirectory += (carpeta+"/");
					verificacion = false;
				}
				

			}else if(ingreso=="clear"){
				//limpia la consola
				for (int i = 0; i < 25; i++){
					cout<<endl<<endl;
				}
			}else if(ingreso=="rmdir"){


			}else if(substring=="rmdir"){
				//elimina un directorio si este esta vacio
				string carpeta = ingreso.substr(cont+1,ingreso.size());
				string path = currentDirectory+carpeta;
				rmdir(path.c_str());
			}else if(ingreso=="ls"){
				//crea rutas simbolicas y duras
				string dir = currentDirectory;
				vector<string> files = vector<string>();
				getdir(dir,files);
				for (unsigned int i = 0;i < files.size();i++)
				{
				cout << files[i] << endl;
				}
			}else if(ingreso=="cat"){
				cout<<"Al comando le faltan argumentos"<<endl;
			}else if(substring=="cat"){
				//Imprime los datos de un archivo en la consola o redirecciona la entrada de datos a un archivo. 
				//Use tambien este comando para crear un archivo usando ctrl-d para el EOF
				string nameArch = ingreso.substr(cont+1,ingreso.size());
				char nombreArchivo[200];
				//cin >> nombreArchivo;
				strcpy(nombreArchivo, nameArch.c_str());
				stringstream ss;
				ss<<currentDirectory<<nombreArchivo;
				ss>>nombreArchivo;
				string concatName;
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
			}else if(ingreso=="rm"){
				cout<<"Al comando le faltan argumentos"<<endl;
			}else if(substring=="rm"){
				//elimina un archivo
				string nameArch = ingreso.substr(cont+1,ingreso.size());
				string path = currentDirectory+nameArch;

				if( remove( path.c_str() ) != 0 )
				    perror( "Error deleting file" );
				else
				    puts( "File successfully deleted" );
				
			}else if(ingreso=="ln"){
				//ln –s: crea rutas simbolicas y duras
			}else if(ingreso=="ps"){
				//ps –a –u –x -e: imprime la lista de los procesos
				struct dirent **namelist;
			    int n;

			    n = scandir("/proc", &namelist, filter, 0);
			    if (n < 0)
				  perror("Not enough memory.");
			    else {
				    while(n--) {
				       	processdir(namelist[n]);
				       	free(namelist[n]);
				  	}
				  	free(namelist);
			    }
				
			}else if(ingreso=="uname"){
				//uname –a –r -m -s: muestra información general del sistema
				
			}else if(ingreso=="kill"){
				//kill -9 process_id (debe usar la llamada al sistema signal())
				
			}else if(ingreso=="exit"){
				//se sale de la sesión de consola
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