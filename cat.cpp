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
	//Imprime los datos de un archivo en la consola o redirecciona la entrada de datos a un archivo.
	//Use tambien este comando para crear un archivo usando ctrl-d para el EOF

  	char CDIR[1024];
	//cout << getcwd(CDIR,sizeof(CDIR)) << endl;
  	//cout<<"YUPIII"<<loquerecibo<<"FinYUPI"<<endl;

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

	bool entro = false;
	string Arch = ruta[1];
	for (int i = 0; i < Arch.size(); ++i)
	{
		if (Arch[i]=='>')
		{
			entro = true;
			break;
		}
	}
	if (entro)
	{
		char* duplicado2 = strdup(Arch.c_str());
		char* token2 = strtok(duplicado2, ">");
		while(token2 != NULL){
			archivos.push_back(ruta[0]+string(token2));
			// the call is treated as a subsequent calls to strtok:
			// the function continues from where it left in previous invocation
			token2 = strtok(NULL, ">");
		}

	}
	cout<<endl<<archivos[0]<<endl<<archivos[1]<<endl;
	


	char recibo[200] = "origen.txt>dos.txt";
	//string recibo = argv[1];
	string prueba = "";
	string origen = "";
	string destino = "";
	int counter = 0;

	//cout << "RECIBO: "<<recibo<<endl;

	for(int i = 0;i < strlen(recibo) ;i++){
		prueba[i] = recibo[i];
		counter++;
	}

	string parametro = "";
	bool redireccionar = false;
	bool append = false;

for(int i = 0;i < counter; i ++){
		parametro += prueba[i];
		if(prueba[i] == '>'){
				redireccionar = true;
			if(prueba[i+1] == '>'){
				redireccionar = true;
				append = true;
			}
		}
	}

	int firstDelimiter = 0;
	int numberUntilSecondDelim = 0;
	int continueOnCharNumber = 0;

	for(int i = 0;i < counter; i ++){
		if(prueba[i] == '>'){
			if(prueba[i+1] == '>'){
				numberUntilSecondDelim = continueOnCharNumber + 2;
				continueOnCharNumber = counter - numberUntilSecondDelim;
				break;
			}
		}else{
			firstDelimiter++;
			continueOnCharNumber++;
			numberUntilSecondDelim++;
		}
	}

	//cout << "Counter: " << counter <<endl;
	//cout << "firstDelimiter: " << firstDelimiter <<endl;
	//cout << "continueOnCharNumber: " << continueOnCharNumber <<endl;
	//cout << "numberUntilSecondDelim: " << numberUntilSecondDelim <<endl;

		for(int i = 0;i < firstDelimiter; i ++){
			origen += prueba[i];
		}

		int initDestino = 0;
		for(int i = numberUntilSecondDelim;i < counter; i++){
			destino += prueba[i];
			initDestino++;
		}

		int longOrigen = 0;
		for (int i = 0; i < firstDelimiter; i++) {
			longOrigen++;
		}

		//cout << "Longitud Origen: " << longOrigen <<endl;
		//cout << "First Param: " << origen.c_str()<<endl;
		//cout << "Second Param: " << destino.c_str()<<endl;


	//cout << "Counter size: " << counter <<endl;
	//cout << parametro <<endl;

	string originContent = "";
	string destinyContent = "";

	if(redireccionar && append){

		string lineOrigin;
		ifstream archivoOri(origen.c_str(), ios::in);
		//archivo.open();
		if (archivoOri.is_open()){
			while(getline(archivoOri,lineOrigin)){
				originContent += lineOrigin += '\n';
			}
			archivoOri.close();
		}

		string lineDestiny;
		ifstream archivoDestin(destino.c_str(), ios::in);
		//archivo.open();
		if (archivoDestin.is_open()){
			while(getline(archivoDestin,lineDestiny)){
				destinyContent += lineDestiny += '\n';
			}
			archivoDestin.close();
		}

		ofstream myfile (destino.c_str(),ios::app);
  	if (myfile.is_open())
  	{
    	myfile << originContent << '\n';
    	myfile.close();
  	}

		originContent = "";
		destinyContent = "";
		origen = "";
		destino = "";

	}else{

		origen = "";
		destino = "";
		//cout << "-----------------------------------"<<endl;

		prueba = "";

		for(int i = 0;i < strlen(recibo) ;i++){
			prueba += recibo[i];
		}

		//cout << "PRUEEEEBAAAA: "<<prueba<<endl;

		int firstDelimiterN = 0;
		int numberUntilSecondDelimN = 0;
		int continueOnCharNumberN = 0;

		for(int i = 0;i < counter; i ++){
			if(prueba[i] == '>'){
					numberUntilSecondDelimN = continueOnCharNumberN + 1;
					continueOnCharNumberN = counter - numberUntilSecondDelimN;
					break;
			}else{
				firstDelimiterN++;
				continueOnCharNumberN++;
				numberUntilSecondDelimN++;
			}
		}

		//cout << "Counter: " << counter <<endl;
		//cout << "firstDelimiterNNN: " << firstDelimiterN <<endl;
		//cout << "continueOnCharNumberNNN: " << continueOnCharNumberN <<endl;
		//cout << "numberUntilSecondDelimNNN: " << numberUntilSecondDelimN <<endl;

		for(int i = 0;i < firstDelimiterN; i++){
			origen += prueba[i];
		}

		int initDestino = 0;
		for(int i = numberUntilSecondDelimN;i < counter; i++){
			destino += prueba[i];
			initDestino++;
		}

		//cout << "origenNNN: "<<origen<<endl;
		//cout<<"destinoNNNN: "<<destino<<endl;

		string lineOriginN;
		ifstream archivoOriN(origen.c_str(), ios::in);
		//archivo.open();
		if (archivoOriN.is_open()){
			while(getline(archivoOriN,lineOriginN)){
				originContent += lineOriginN += '\n';
			}
			archivoOriN.close();
		}

		//cout << "originContent: " <<originContent <<endl;

		ofstream myfileN (destino.c_str(), ios::out);
		if (myfileN.is_open())
		{
			myfileN << originContent << '\n';
			myfileN.close();
		}

		originContent = "";
		destinyContent = "";


	}

if(!redireccionar){
	string line;
	ifstream archivo(argv[0], ios::in);
	//archivo.open();
	if (archivo.is_open()){
		while(getline(archivo,line)){
			cout<<line<<endl;
		}
		archivo.close();
	}
}

return 0;

/*
	string line;
	ifstream archivo(argv[0], ios::in);
	//archivo.open();
	if (archivo.is_open()){
		while(getline(archivo,line)){
			cout<<line<<endl;
		}
		archivo.close();
	}
	return 0;
	*/
}
