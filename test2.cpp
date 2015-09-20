#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
using namespace std;
char gpath[1024];
typedef void (*sighandler_t)(int);
//function to handle signal
void handle_signal(int signo)
{
	printf("\nmi_sh@%s>",gpath);
	fflush(stdout);
}
//function for single pipe implementation
int simple_implementation(char *s,int len)
{
    int j=1,status;
    int pid;
    char *t=strtok(s," ");
    char *argv[100];
    argv[0]=t;
    while(t!=NULL)
    {
        t=strtok(NULL," ");
        if(t==NULL)
            break;
        argv[j]=t;
        j++;
    }
    argv[j]=NULL;
    pid=fork();
    if(pid==0)
    {
        execvp(*argv,argv);
        fprintf(stderr," %s :COMMAND NOT FOUND\n",*argv);
    }
    else if (pid>0)
    {
          wait(&status);
    }

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

int main()
{
    char str[1024];
    getcwd(gpath,sizeof(gpath));
    char temp_path[1024];
    for(int j=0;j<strlen(gpath);j++)
    {
        temp_path[j]=gpath[j];
    }
    char *his_path=getenv("HOME");
    char hispath[1024];
    for(int j=0;j<strlen(his_path);j++)
    {
        hispath[j]=his_path[j];
    }
    strcat(hispath,"/m_history.txt");
    signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);
	printf("\nmi_sh@%s>",gpath);
	int ofd,rfd;
	//int count=0;
	while(fgets(str,1024,stdin)!=NULL)
	{
            char BUF[1024];
          //  count++;
            char *t;
            char *s[1024];
            int len=strlen(str);
            if(!strstr(str,"!"))
            {

                ofd= open(hispath, O_WRONLY|O_CREAT|O_APPEND,0700);
                int w=write(ofd,str,strlen(str));
                close(ofd);
            }
            str[strlen(str)-1]='\0';
            if(strstr(str,"!"))    // bang (!) operator implementation
            {
                string his_arr[1024];
                int h=0;
                static const char *f =hispath;
                FILE *file = fopen ( f, "r" );
                if ( file != NULL )
                {
                    char line_by_line [ 1024 ];
                    while ( fgets ( line_by_line, sizeof line_by_line, file ) != NULL )
                    {
                        his_arr[h]=line_by_line;
                        h++;
                    }
                fclose ( file );
                }
                char bang_temp[100];
                char temp[100];
                int t=0;
                strcpy(bang_temp,str);
                if(bang_temp[1]=='+')
                {
                    for(int c=2;c<strlen(bang_temp);c++)
                    {
                        temp[t]=bang_temp[c];
                        t++;
                    }
                    int cmd=atoi(temp);
                    cmd--;
                    strcpy(str,his_arr[cmd].c_str());
                    ofd= open(hispath, O_WRONLY|O_CREAT|O_APPEND,0700);
                    int w=write(ofd,str,strlen(str));
                    close(ofd);
                    str[strlen(str)-1]='\0';
                }
                else if(bang_temp[1]=='-')
                {
                    for(int c=2;c<strlen(bang_temp);c++)
                    {
                        temp[t]=bang_temp[c];
                        t++;
                    }
                    int cmd=atoi(temp);
                    strcpy(str,his_arr[h-cmd].c_str());
                    ofd= open(hispath, O_WRONLY|O_CREAT|O_APPEND,0700);
                    int w=write(ofd,str,strlen(str));
                    close(ofd);
                    str[strlen(str)-1]='\0';
                }
                else if(bang_temp[1]=='!')
                {

                    strcpy(str,his_arr[h-1].c_str());
                    ofd= open(hispath, O_WRONLY|O_CREAT|O_APPEND,0700);
                    int w=write(ofd,str,strlen(str));
                    close(ofd);
                    str[strlen(str)-1]='\0';
                }
                else if(isalpha(bang_temp[1]))
                {
                    for(int c=1;c<strlen(bang_temp);c++)
                    {
                        temp[t]=bang_temp[c];
                        t++;
                    }

                    for(int c=h+1;c>=0;c--)
                    {

                       for(int d=0;d<t;d++)
                        {
                            if(temp[d]==his_arr[c][d])
                            {
                                if(d==t-1)
                                {
                                    strcpy(str,his_arr[c].c_str());
                                    ofd= open(hispath, O_WRONLY|O_CREAT|O_APPEND,0700);
                                    int w=write(ofd,str,strlen(str));
                                    close(ofd);
                                    str[strlen(str)-1]='\0';
                                    break;
                                }
                            }
                            else
                                break;
                        }
                    }

                }
                else
                {
                    for(int c=1;c<strlen(bang_temp);c++)
                    {
                        temp[t]=bang_temp[c];
                        t++;
                    }
                    int cmd=atoi(temp);
                    cmd--;
                    strcpy(str,his_arr[cmd].c_str());
                    ofd= open(hispath, O_WRONLY|O_CREAT|O_APPEND,0700);
                    int w=write(ofd,str,strlen(str));
                    close(ofd);
                    str[strlen(str)-1]='\0';
                }
           }
            if(strstr(str,"history")) //history command implementation
            {
                static const char *f =hispath;
                FILE *file = fopen ( f, "r" );
                if ( file != NULL )
                {
                    char line_by_line [ 1024 ];
                    int i=1;
                    while ( fgets ( line_by_line, sizeof line_by_line, file ) != NULL )
                    {
                        printf("%02d. ",i);
                        fputs ( line_by_line, stdout );
                        i++;
                    }
                fclose ( file );
                }
            }
            if(strstr(str,"cd")) //cd command implementation
            {
                t=strtok(str," ");
                t=strtok(NULL," ");
                if(t!=NULL)
                {
                 chdir(t);
                 getcwd(gpath,sizeof(gpath));
                }
                else
                 {
                    chdir(his_path);
                    getcwd(gpath,sizeof(gpath));
                 }
            }
            if(strstr(str,"exit"))
            {
                printf("bye..\n");
                exit(0);
            }
            if(strstr(str,"echo")) // basic predefined environment variable implementation
            {
                if(strstr(str,"$"))
                {
                    t=strtok(str,"$");
                    t=strtok(NULL,"$");
                    cout<<getenv(t);
                }
            }
            char *present=strstr(str,"<");
            if(present!=NULL)
            {
                if(strstr(str,"|"))
                {

                    if(strstr(str,">"))
                    {
                        t=strtok(str,"<");
                        char *inred[100];
                        inred[0]=t;
                        t=strtok(NULL,"<");
                        char *t2=strtok(t,"|");
                        inred[1]=t2;
                        s[0]=t2;
                        int i=1;
                        char *outred;
                        while(t2!=NULL)
                        {
                            t2=strtok(NULL,"|");
                            if(t2==NULL)
                                break;
                            s[i]=t2;
                            i++;
                        }
                        char *temp=s[i-1];
                        char *t3=strtok(temp,">");
                        s[i-1]=t3;
                        t3=strtok(NULL,">");
                        outred=t3;

                        input_pipe_output(inred[0],inred[1],s,outred,i-1);
                    }
                    else
                    {
                        t=strtok(str,"<");
                        char *inred[100];
                        inred[0]=t;
                        t=strtok(NULL,"<");
                        char *t2=strtok(t,"|");
                        inred[1]=t2;
                        s[0]=t2;
                        int k=1;
                        while(t2!=NULL)
                        {
                            t2=strtok(NULL,"|");
                            if(t2==NULL)
                                break;
                            s[k]=t2;
                            k++;
                        }
                         input_pipe(inred[0],inred[1],s,k-1);
                    }
                }
               else if(strstr(str,">"))
                   {
                        t=strtok(str,"<");
                        s[0]=t;
                        t=strtok(NULL,"<");
                        char *t2=strtok(t,">");
                        s[1]=t2;
                        t2=strtok(NULL,">");
                        s[2]=t2;
                        input_output(s[0],s[1],s[2]);
                   }
                    else
                    {
                        t=strtok(str,"<");
                        s[0]=t;
                        t=strtok(NULL,"<");
                        s[1]=t;
                        single_input_redirect(s[0],s[1]);
                    }
            }
            else if(strstr(str,">"))
            {
                if(strstr(str,"|"))
                {
                    t=strtok(str,"|");
                    s[0]=t;
                    int i=1;
                    char *outred;
                    while(t!=NULL)
                    {
                    t=strtok(NULL,"|");
                    if(t==NULL)
                        break;
                        //cout<<t;
                    s[i]=t;
                    i++;
                    }
                    char *temp=s[i-1];
                    char *t2=strtok(temp,">");
                    s[i-1]=t2;
                    t2=strtok(NULL,">");
                    outred=t2;
                    out_pipe(s,outred,i-1);
                }
                 else
                 {
                    t=strtok(str,">");
                    s[0]=t;
                    t=strtok(NULL,">");
                    s[1]=t;
                    single_output_redirect(s[0],s[1]);
                }
            }
           else if(strstr(str,"|"))
            {
                t=strtok(str,"|");
                s[0]=t;
                int i=1;
                while(t!=NULL)
                {
                    t=strtok(NULL,"|");
                    if(t==NULL)
                        break;
                    s[i]=t;
                    i++;
                }
           mul_pipe(s,i-1);
           }
           else
           {
                if(!(strstr(str,"exit")||strstr(str,"history")||strstr(str,"cd")))
                 {
                         s[0]=str;
                         simple_implementation(s[0],0);
                 }
           }
            printf("mi_sh@%s>",gpath);

	}
}