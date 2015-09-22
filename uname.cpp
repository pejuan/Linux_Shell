#include <iostream>
#include <sys/utsname.h>

using namespace std;

int main(int argc, char const *argv[]){
	struct utsname sysinfo;
	uname(&sysinfo);
	string demas = argv[0];

	if (demas == "-a")
	{
		cout << sysinfo.sysname<<endl;//S
		cout << sysinfo.nodename<<endl;
		cout << sysinfo.release<<endl;//R
		cout << sysinfo.version<<endl;
		cout << sysinfo.machine<<endl; //M
		//cout << "Domain Name: "<<sysinfo.domainname<<endl;
	}
	else if (demas == "-s")
	{
		cout <<sysinfo.sysname<<endl;//S
	}
	else if (demas == "-r")
	{
		cout << sysinfo.release<<endl;//R
	}
	else if (demas == "-m")
	{
		cout << sysinfo.machine<<endl; //M
	}
	else if (demas == "-s -r" || demas == "-r -s")
	{
		cout << sysinfo.sysname<<endl;//S
		cout << sysinfo.release<<endl;//R
	}
	else if (demas == "-s -m" || demas == "-m -s")
	{
		cout << sysinfo.sysname<<endl;//S
		cout << sysinfo.machine<<endl; //M
	}
	else if (demas == "-m -r" || demas == "-r -m")
	{
		cout << sysinfo.machine<<endl; //M
		cout << sysinfo.release<<endl;//R
	}
	else if (demas == "-s -r -m" || demas == "-s -m -r" || demas == "-r -s -m" || demas == "-r -m -s" || demas == "-m -s -r" || demas == "-m -r -s") 
	{
		cout << sysinfo.sysname<<endl;//S
		cout << sysinfo.machine<<endl; //M
		cout << sysinfo.release<<endl;//R
	}

return 0;
}