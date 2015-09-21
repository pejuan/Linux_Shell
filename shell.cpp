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
#include <sys/utsname.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

using namespace std;

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

int salida_a_pipe(char *s[],char *outred,int n)
{
    int pip[2*n];
    for(int i=0;i<2*n;i=i+2)
    {
        pipe(pip+i);
    }
    int status;
    int out;
    char *dest=strtok(outred," ");
    out = open(dest, O_WRONLY | O_TRUNC | O_CREAT|O_APPEND,0700);
    for(int j=0;j<n+1;j++)
    {
        char *t;
        char *argv[1024];
        t=strtok(s[j]," ");
        argv[0]=t;
        int m=1;
        while(t!=NULL)
        {
        t=strtok(NULL," ");
        if(t==NULL)
            break;
        argv[m]=t;
        m++;
        }
        argv[m]=NULL;
        if(fork()==0)
        {
            if(j==0)
            {
                dup2(pip[1],1);
                for(int k=0;k<2*n;k++)
                    close(pip[k]);
                 execvp(*argv, argv);
            }
           else if(j==n)
            {
                dup2(pip[2*n-2],0);
                dup2(out,1);

                    for(int k=0;k<2*n;k++)
                    close(pip[k]);
                  close(out);
                 execvp(*argv, argv);
            }
            else
            {
                dup2(pip[2*j+1],1);
                dup2(pip[2*(j-1)],0);
                for(int k=0;k<2*n;k++)
                    close(pip[k]);
                 execvp(*argv, argv);
            }
        }
    }
     for(int k=0;k<2*n;k++)
        close(pip[k]);
     close(out);
    for(int k=0;k<2*n-1;k++)
        wait(&status);
}

int redireccion_de_salida_unica(char *s1,char *s2)
{
    char *t=strtok(s1," ");
    char *argv[100];
    argv[0]=t;
    int i=1,status;
    while(t!=NULL)
    {
        t=strtok(NULL," ");
        if(t==NULL)
            break;
        argv[i]=t;
        i++;
    }
    argv[i]=NULL;
    char *dest=s2;
    int out;
    out = open(dest, O_WRONLY | O_TRUNC | O_CREAT|O_APPEND,0700);
    if (fork() == 0)
    {
        dup2(out,1);
        close(out);
        execvp(*argv,argv);
    }

     close(out);
  for (int i = 0; i < 1; i++)
    wait(&status);

}
 int entrada_pipe_salida(char *inred,char *p,char *s[],char *outred,int n)
 {
    int in,status;
    char *inargv[100];
    char *t3=strtok(inred," ");
    inargv[0]=t3;
    int k=1;
    while(t3!=NULL)
    {
        t3=strtok(NULL," ");
        if(t3==NULL)
            break;
        inargv[k]=t3;
        k++;
    }
    inargv[k]=NULL;
    char *path=strtok(p," ");
    in=open(path,O_RDONLY);
    int pip[2*n];
    for(int i=0;i<2*n;i=i+2)
    {
        pipe(pip+i);
    }
    int out;
    char *dest=strtok(outred," ");
    out = open(dest, O_WRONLY | O_TRUNC | O_CREAT|O_APPEND,0700);
    for(int j=0;j<n+1;j++)
    {
        char *t;
        char *argv[1024];
        t=strtok(s[j]," ");
        argv[0]=t;
        int m=1;
        while(t!=NULL)
        {
        t=strtok(NULL," ");
        if(t==NULL)
            break;
        argv[m]=t;
        m++;
        }
        argv[m]=NULL;
        if(fork()==0)
        {
            if(j==0)
            {
               dup2(in,0);
               dup2(pip[1],1);
                for(int k=0;k<2*n;k++)
                    close(pip[k]);
                close(in);
                 execvp(*inargv,inargv);
            }
           else if(j==n)
            {
                dup2(pip[2*n-2],0);
                dup2(out,1);

                    for(int k=0;k<2*n;k++)
                    close(pip[k]);
                  close(out);
                 execvp(*argv, argv);
            }
            else
            {
                dup2(pip[2*j+1],1);
                dup2(pip[2*(j-1)],0);
                for(int k=0;k<2*n;k++)
                    close(pip[k]);
                 execvp(*argv, argv);
            }
        }
    }
     for(int k=0;k<2*n;k++)
        close(pip[k]);
    close(in);
    close(out);
    for(int k=0;k<2*n-1;k++)
        wait(&status);

}

char CDIR[1024]; 

int main(int argc, char const *argv[])
{

	pid_t pid;

	int fd[2];
	pipe(fd);

	
	vector<string> listaComandos;
	bool verificacion = true;
	//string currentDirectory = "/Users/jmlb/";
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
	getcwd(CDIR,sizeof(CDIR));
	string currentDirectory = CDIR; //Añadiendo esto para que el currentDirectory sea dinamico -jc

	while(true){

		cout<<"mi_sh@"<<currentDirectory<<"> ";

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

		if (substring ==  "mkdir")
		{

			string dirName = currentDirectory + ingreso.substr(cont+1,ingreso.size());
			char* fileName[] = {(char*) dirName.c_str(), (char*)0};
			
			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/mkdir";
				execv(ejecutable.c_str(),fileName);
			}

		}
		else if (substring == "cat")
		{
			string dirName = currentDirectory + ingreso.substr(cont+1,ingreso.size());
			char* fileName[] = {(char*) dirName.c_str(), (char*)0};

			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/cat";
				execv(ejecutable.c_str(),fileName);
			}

		}
		else if(ingreso=="exit"){
				//se sale de la sesión de consola
				break;
		}
		else if(ingreso=="clear"){
				//limpia la consola
				for (int i = 0; i < 25; i++){
					cout<<endl<<endl;
				}
				cout << "\e[H\e[2J";
		}
		else if (ingreso =="ls")
		{
			
			if (!fork()) {
				string ejecutable = CDIR;
				char* directory[] = {(char*) currentDirectory.c_str(), (char*)0};
				string comando = ejecutable + "/ls";
				//ejecutable += "/ls";
				execv(comando.c_str(),directory);
			}
		}
		else if (substring == "rmdir")
		{
			string dirName = currentDirectory + ingreso.substr(cont+1,ingreso.size());
			char* fileName[] = {(char*) dirName.c_str(), (char*)0};
			
			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/rmdir";
				execv(ejecutable.c_str(),fileName);
			}
		}
		else if (ingreso == "ps")
		{
			if (!fork()) {
				execlp("ps","ps",NULL);
			}
		}
		else if (substring == "ps")
		{
			string dirName = ingreso.substr(cont+1,ingreso.size());
			char* bandera[] = {(char*) dirName.c_str(), (char*)0};
			
			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/ps";
				execv(ejecutable.c_str(),bandera);
			}
		}
		else if (ingreso == "rm")
		{
			cout << "Faltan argumentos, uso: rm nombre_archivo" << endl;	
		}	
		else if (substring == "rm")
		{
			string dirName = currentDirectory + ingreso.substr(cont+1,ingreso.size());
			char* fileName[] = {(char*) dirName.c_str(), (char*)0};
			
			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/rm";
				execv(ejecutable.c_str(),fileName);
			}
		}
		else if (ingreso == "uname")
		{
			struct utsname sysinfo;
			uname(&sysinfo);
			cout << "System Name: "<<sysinfo.sysname<<endl;//S
		}
		else if (substring == "uname")
		{
			string dirName = ingreso.substr(cont+1,ingreso.size());
			char* bandera[] = {(char*) dirName.c_str(), (char*)0};
			
			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/uname";
				execv(ejecutable.c_str(),bandera);
			}
		}
		else if(ingreso=="cd"){
				//cambiar de directorio
				currentDirectory = "/Users/jmlb/"; //getenv("HOME"); deberia de retornar el home dir, no funciona en macos
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
			}
		}
		else if (substring == "chmod")
		{
			int c =0;
			string substring2 = ingreso.substr(cont+1,ingreso.size()); //permiso y nombre de archivo
			for (int i = 0; i < substring2.size(); ++i)
			{
				if (substring2[i] == ' ')
				{
					break;
				}
				else{
					c++;
				}
			}
			string permission = substring2.substr(0, c);
			string archivo = substring2.substr(c+1,substring2.size());
			string concat = permission + '~' + currentDirectory + archivo; 

			if (!fork()) {
				string ejecutable = CDIR;
				char* todo[] = {(char*) concat.c_str(), (char*)0};
				string comando = ejecutable + "/chmod";
				//ejecutable += "/ls";
				execv(comando.c_str(),todo);
			}
		}
	} // fin while true
	
	return 0;
}