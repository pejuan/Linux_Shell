//Linux Shell Implementation 
//Supports command line arguments
//Supports signal handling 
//Supports INPUT/OUTPUT redirection

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

extern char ** environ;		//environment variables
char * user;		//username to be displayed in the shell 

//Function Declarations
void handle_SIGINT (int sig);		//handles SIGINT signal {ctrl + C key}
vector <string> parse_string (string temp, char delim);	//parses the string according to delimiter delim
string attach (vector <string> path, string cmd);		//searches the path for the command and appends it to string 'cmd'
string attach_path (string cmd);
void execute (const char * a_path, vector <string> command);	//executes the command using the attached path
void redirect_io (string temp, char c);		//input redirection using '<' and '>' operators


int main()
{
	signal (SIGINT, SIG_IGN);
	signal (SIGINT, handle_SIGINT);		//ctrl+c signal handler	
	char c_input = '\0';		//character input from shell
	vector <string> command;
	string temp, cmd, a_path;
	user = getlogin ();		//fetches username for display in shell  
	const char *a_path1;	//attached path string	
	cout << user << ":" << "Shell" << ":";
	while (c_input != EOF)		//Infinite loop for taking input character from command line
	{
		c_input = getchar ();
		if (c_input == '\n')	//parse the string and execute command
		{	 
			if (temp.size () != 0)
			{
				size_t found;	//flag variable for input redirection operators 
				int check = 0;	//keeps check to carry out only one IO redirection 
				found = temp.find (">>");
				if (found != string::npos)		//if >> character found
				{
					redirect_io (temp, '#');		//redirect input output 
					check = 1;
				}
				found = temp.find ("<");
				if (check == 0 && found != string::npos)		//if < character found
				{
					redirect_io (temp, '<');		//redirect input output 
					check = 1;
				}
				found = temp.find (">");
				if (check == 0 && found != string::npos)		//if > character found
				{
					redirect_io (temp, '>');		//redirect input output 
					check = 1;
				}

				if (check == 0)			//if none of above characters found
				{
					command = parse_string (temp, ' ');		//parse the entered string
					cmd = command [0];
					found = cmd.find ("/");
					a_path = cmd;
					if (found == string::npos)			//if '/' not found then user has entered only the command
						a_path = attach_path (cmd);		//attach full path to the command
					a_path1 = (a_path).c_str ();	//convert string to array of characters
					execute (a_path1, command);	 	//execute the command
				}	
			}
			cout << user << ":" << "Shell" << ":";
			temp.clear ();
		}	
		else		//concatenate string
			temp += c_input;
	}
}


void handle_SIGINT (int sig)		//handling ctrl+C signal
{
	cout << endl << user << ":" << "Shell" << ":";
	fflush (stdout);
}


//parses string with delimiter delim and returns a vector of strings
vector <string> parse_string (string temp, char delim)
{
	string buf;
	stringstream ss (temp);
	vector <string> parsed;
	while (getline (ss, buf, delim))	//reads the stringstream 'ss' until it finds delim and stores it in 'buf'
	{
		if (buf.size () > 0)		
			parsed.push_back (buf);		//appends buf to string vector
	}
	return parsed;
}


//returns path to be attached to 'cmd' command
string attach (vector <string> path, string cmd)
{
	DIR *dir;
	struct dirent *ent;
	const char* cmd1;
	for (vector <string>::iterator it = path.begin (); it != path.end (); ++it)	//iterates through addresses in path vector
	{
		cmd1 = (*it).c_str ();		
		if ((dir = opendir (cmd1)) != NULL)
		{
			while ((ent = readdir (dir)) != NULL) 	//iterats through all the existing paths in the path cmd1
			{
    			if(ent -> d_name == cmd)
    				return *it;
  			}
  			closedir (dir);
		} 
		else 
  			return "";		 
	}
}


//Attach path if the command entered is not full path
string attach_path (string cmd)
{
	string penv;	//string containing environment variable path
	penv = getenv ("PATH");
	string a_path;	//string with path attached
	vector <string> path;
	path = parse_string (penv, ':');		//all the paths stored in the vector path
	a_path = attach (path, cmd);			//returns path that needs to be attached
	a_path += '/';	
	a_path += cmd;							//attaches command to the path returned by attach ()
	return a_path;
}


//execute the command after attaching the path
void execute (const char *a_path, vector <string> command)
{
    const char *programname = a_path;		//create array of pointers using vector
    const char **argv = new const char* [command.size()+1];   // extra space for sentinel
    argv [0] = programname;         // by convention, argv[0] is program name
    int i, j;
    for (j = 1;  j < command.size ();  ++j)     // copy args
            argv [j] = command [j] .c_str ();

    argv [j] = NULL;  // end of arguments sentinel is NULL
	//execute the command using execve
	if (fork () == 0) 
	{
		i = execve (programname, (char**) argv, environ);
		if (i < 0) 
		{
			printf ("%s: %s\n", a_path, "command not found");
			exit (1);		
		}
	} 
	else
		wait(NULL);
}


void redirect_io (string temp, char c)
{
	vector <string> parsed;
	const char *programname, *filename;
	const char **argv = new const char* [2]; 
	cout << c << endl;
	if(c != '#' || c!='|')
	{
		parsed = parse_string (temp, c);
		programname = (parsed [0]).c_str ();
	    filename = (parsed [1]).c_str ();
	    argv [0] = programname;         // by convention, argv[0] is program name
	    argv [1] = NULL;  // end of arguments sentinel is NULL
	}
	if (fork () == 0)		//fork a child process to carry out redirection
	{
		if (c == '<')
		{
			int fd0 = open (filename, O_RDONLY, 0);	 //opens filename
			dup2 (fd0, 0);		//duplicates the file descriptor at STDIN stream '0' //Input gets redirected 
			int i;
			i = execve (programname, (char**)argv, environ);
			if (i < 0) 
			{
				printf ("%s: \n", "comman not found");
				exit (1);		
			}	
		}
		else if (c == '>')
		{
        	int fd1 = creat (filename, 0644);	//creates filename in write mode
        	dup2 (fd1, 1);		//duplicates the file descriptor at STDOUT stream '1' //Output gets redirected
			int i;
			i = execve (programname, (char**)argv, environ);
			if (i < 0) 
			{
				printf ("%s: \n", "comman not found");
				exit (1);		
			}	
		}
		else if (c == '#')
		{
			parsed = parse_string (temp, '>');
			programname = (parsed [0]).c_str ();
		    filename = (parsed [1]).c_str ();
		    argv [0] = programname;         // by convention, argv[0] is program name
		    argv [1] = NULL;  // end of arguments sentinel is NULL
			int fd1 = open(filename, O_CREAT|O_RDWR|O_APPEND, 0); //opens the filename in append mode
			dup2 (fd1, 1);
			int i;
			i = execve (programname, (char**) argv, environ);
			if (i < 0) 
			{
				printf ("%s: \n", "comman not found");
				exit (1);		
			}	
		}
	}
	else
	{		
		wait (NULL);
		fflush (0);
	}
}