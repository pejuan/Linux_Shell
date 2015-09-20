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


int main(int argc, char const *argv[])
{
	char CDIR[1024];
	getcwd(CDIR, sizeof(CDIR));
	string dir = argv[0];
	vector<string> files = vector<string>();
	getdir(dir,files);
	for (unsigned int i = 0;i < files.size();i++)
	{
		cout << files[i] << endl;
	}

	return 0;
}