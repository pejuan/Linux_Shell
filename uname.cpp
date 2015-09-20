#include <iostream>
#include <sys/utsname.h>

using namespace std;

int main(int argc, char const *argv[]){
	struct utsname sysinfo;
	uname(&sysinfo);
	string demas = argv[0];

	if (demas == "-a")
	{
		cout << "System Name: "<<sysinfo.sysname<<endl;//S
		cout << "Host Name: "<<sysinfo.nodename<<endl;
		cout << "Release(Kernel) Version: "<<sysinfo.release<<endl;//R
		cout << "Kernel Build Timestamp: "<<sysinfo.version<<endl;
		cout << "Machine Arch: "<<sysinfo.machine<<endl; //M
		//cout << "Domain Name: "<<sysinfo.domainname<<endl;
	}
	else if (demas == "-s")
	{
		cout << "System Name: "<<sysinfo.sysname<<endl;//S
	}
	else if (demas == "-r")
	{
		cout << "Release(Kernel) Version: "<<sysinfo.release<<endl;//R
	}
	else if (demas == "-m")
	{
		cout << "Machine Arch: "<<sysinfo.machine<<endl; //M
	}
	else if (demas == "-s -r" || demas == "-r -s")
	{
		cout << "System Name: "<<sysinfo.sysname<<endl;//S
		cout << "Release(Kernel) Version: "<<sysinfo.release<<endl;//R
	}
	else if (demas == "-s -m" || demas == "-m -s")
	{
		cout << "System Name: "<<sysinfo.sysname<<endl;//S
		cout << "Machine Arch: "<<sysinfo.machine<<endl; //M
	}
	else if (demas == "-m -r" || demas == "-r -m")
	{
		cout << "Machine Arch: "<<sysinfo.machine<<endl; //M
		cout << "Release(Kernel) Version: "<<sysinfo.release<<endl;//R
	}
	else if (demas == "-s -r -m" || demas == "-s -m -r" || demas == "-r -s -m" || demas == "-r -m -s" || demas == "-m -s -r" || demas == "-m -r -s") 
	{
		cout << "System Name: "<<sysinfo.sysname<<endl;//S
		cout << "Machine Arch: "<<sysinfo.machine<<endl; //M
		cout << "Release(Kernel) Version: "<<sysinfo.release<<endl;//R
	}

return 0;
}