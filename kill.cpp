#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int procID = atoi(argv[0]);
	pid_t pid = (pid_t)procID;
	kill(pid,SIGKILL);
	return 0;
}
