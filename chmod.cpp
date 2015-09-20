#include <sys/stat.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	
	string recibe = argv[0];
	int c = 0;
	for (int i = 0; i < recibe.size(); ++i)
	{
		if (recibe[i] == '~')
		{
			break;
		}
		else
		{
			c++;
		}
	}

	string argumento = recibe.substr(0,c);
	const char* argumentoc = argumento.c_str();
	string path1 = recibe.substr(c+1, recibe.size());
	const char* path = path1.c_str();
	//char* bandera[] = {(char*) dirName.c_str(), (char*)0};
	char user = argumento[0];
	char group = argumento[1];
	char other = argumento[2];
	int octal = strtol(argumento.c_str(),0,8);
	chmod(path, octal);

/*	
	// bloque USER
	if (argumento[0]== '7'){
		chmod(path, S_IRWXU); //rwx owner
		
	}else if(argumento[0]== '6'){
		chmod(path, S_IRUSR | S_IWUSR); //rw owner
		//chmod(path, S_IWUSR); //w owner
		cout << "modificando rw en owner" << endl;

	}else if(argumento[0]=='5'){
		chmod(path, S_IRUSR); //r owner
		chmod(path, S_IXUSR); //x owner

	}else if(argumento[0]=='4'){
		chmod(path, S_IRUSR); //r owner
		
	}else if(argumento[0]=='3'){
		chmod(path, S_IWUSR); //w owner
		chmod(path, S_IXUSR); //x owner
		
	}else if(argumento[0]=='2'){
		chmod(path, S_IWUSR); //w owner
		
	}else if(argumento[0]=='1'){
		chmod(path, S_IXUSR); //x owner
	}

	// bloque GROUP

	if (argumento[1]=='7'){
		chmod(path, S_IRWXG); //rwx group
		
	}else if(argumento[1]=='6'){
		chmod(path, S_IRGRP); //r group
		chmod(path, S_IWGRP); //w group
		cout << "modificando rw en group" << endl;

	}else if(argumento[1]=='5'){
		chmod(path, S_IRGRP); //r group
		chmod(path, S_IXGRP); //x group
		
	}else if(argumento[1]=='4'){
		chmod(path, S_IRGRP); //r group
		
	}else if(argumento[1]=='3'){
		chmod(path, S_IWGRP); //w group
		chmod(path, S_IXGRP); //x group
		
	}else if(argumento[1]=='2'){
		chmod(path, S_IWGRP); //w group
		
	}else if(argumento[1]=='1'){
		chmod(path, S_IXGRP); //x group
		
	}

	// bloque OTHER

	if (argumento[2]=='7'){
		chmod(path, S_IRWXO); //rwx other
		
	}else if(argumento[2]=='6'){
		chmod(path, S_IROTH); //r other
		chmod(path, S_IWOTH); //w other
		cout << "modificando rw en other" << endl;

	}else if(argumento[2]=='5'){
		chmod(path, S_IROTH); //r other
		chmod(path, S_IXOTH); //x other
		
	}else if(argumento[2]=='4'){
		chmod(path, S_IROTH); //r other
		
	}else if(argumento[2]=='3'){
		chmod(path, S_IWOTH); //w other
		chmod(path, S_IXOTH); //x other
		
	}else if(argumento[2]=='2'){
		chmod(path, S_IWOTH); //w other
		
	}else if(argumento[2]=='1'){
		chmod(path, S_IXOTH); //x other
		
	}

*/

	return 0;
}