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
bool contains(char* comando, vector<string> listaComandos);
bool containsStr(string comando, vector<string> listaComandos);
int input_pipe_output(char *inred,char *p,char *s[],char *outred,int n);
int single_output_redirect(char *s1,char *s2);
int out_pipe(char *s[],char *outred,int n);
int mul_pipe(char *s[],int n);
int input_pipe(char *inred,char *p,char *s[],int n);
int input_output(char *s1,char *s2,char *s3);
int single_input_redirect(char *s1,char *s2);

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
    currentDirectory += "/"

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
		else if (substring == "rmDir")
		{
			string dirName = currentDirectory + ingreso.substr(cont+1,ingreso.size());
			char* fileName[] = {(char*) dirName.c_str(), (char*)0};
			
			if (!fork()) {
				string ejecutable = CDIR;
				ejecutable += "/rmdirR";
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

//function for multiple pipe implementation
int mul_pipe(char *s[],int n)
{
    int pip[2*n];
    for(int i=0;i<2*n;i=i+2)
    {
        pipe(pip+i);
    }
    int status;
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
                    for(int k=0;k<2*n;k++)
                    close(pip[k]);
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
    for(int k=0;k<2*n-1;k++)
        wait(&status);

}
//function for input redirection with single pipe
int input_pipe(char *inred,char *p,char *s[],int n)
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
                    for(int k=0;k<2*n;k++)
                    close(pip[k]);
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
    for(int k=0;k<2*n-1;k++)
        wait(&status);
}
//function for input output redirection
int input_output(char *s1,char *s2,char *s3)
{
   char *t1,t2;
   int status;
   t1=strtok(s1," ");
   char *argv[100];
   argv[0]=t1;
   int i=1;
   while(t1!=NULL)
   {
        t1=strtok(NULL," ");
        if(t1==NULL)
            break;
        argv[i]=t1;
        i++;
   }
   argv[i]=NULL;
   char *path=strtok(s2," ");
   char *dest=strtok(s3," ");
   int in,out;
   in = open(path, O_RDONLY);
   out = open(dest, O_WRONLY | O_TRUNC | O_CREAT|O_APPEND,0700);
    if (fork() == 0)
    {
        dup2(in, 0);
        dup2(out,1);
        close(in);
        close(out);
        execvp(*argv,argv);
    }
    close(in);
    close(out);
    for (int i = 0; i < 2; i++)
      wait(&status);

}
//function for single input redirect
int  single_input_redirect(char *s1,char *s2)
{
    int in,status;
    char *argv[100];
    char *t;
   // t=strtok(s," ");
   // argv5[0]=t5;
    t=strtok(s1," ");
    argv[0]=t;
    int k=1;
    while(t!=NULL)
    {
        t=strtok(NULL," ");
        if(t==NULL)
            break;
        argv[k]=t;
        k++;
    }
    argv[k]=NULL;
   // t=strtok(NULL,"<");
    char *path=strtok(s2," ");
    in=open(path,O_RDONLY);
    if (fork() == 0)
    {

        dup2(in, 0);
        close(in);
        execvp(*argv,argv);
    }

     close(in);



  for (int i = 0; i < 1; i++)
    wait(&status);


}
//function with output redirect with pipe
int out_pipe(char *s[],char *outred,int n)
{
    //cout<<"out red value in function"<<outred;
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
int single_output_redirect(char *s1,char *s2)
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
 int input_pipe_output(char *inred,char *p,char *s[],char *outred,int n)
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