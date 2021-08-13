#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/un.h>
#define PATH "./SOCKNAME"
using namespace std;

int main(int argc, char const *argv[])
{
	int fd = open("bang",O_RDONLY);
	
	while(1)
	{
		char buff[100]={0};
		read(fd,buff,sizeof(buff));
		cout<<buff<<endl;
		sleep(2);
	}
	return 0;
}