#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#define MSIZE 0
using namespace std;

pthread_t rec;
int key = ftok(".",12345);
int msgid = msgget(key,IPC_CREAT|0666);

struct info{
	long pid;
	long gid;
};

struct mybuf {
	long mtype;       /* message type, must be > 0 */
	char mtext[50];    /* message data */
	//struct info in;
	long pid;
	long gid;
};

struct mybuf buf;

void *recv(void *args)
{
	while(1)
	{
		while(msgrcv(msgid,&buf,sizeof(buf),102,IPC_NOWAIT)<=0)	sleep(0.5);	
		//perror("Receive");
		
		cout<<buf.pid<<": "<<buf.mtext<<endl;
	}
}

int main(int c,char *argv[])
{	
	cout<<"I am client: "<<argv[1]<<endl;
	cout<<"Sending join request: "<<endl;
	buf.mtype = 100;	
	strcpy(buf.mtext,"join");
	buf.pid = getpid();
	buf.gid = 0;
	if(-1==msgsnd(msgid,&buf,sizeof(buf),IPC_NOWAIT))	perror("Send");
	
	pthread_create(&rec,NULL,recv,NULL);

	while(strcmp(buf.mtext,"quit")!=0)
	{
		buf.mtype = 101;
		//cout<<"Enter msg: "<<endl;
		cin.getline(buf.mtext,50,'\n');
		if(strcmp(buf.mtext,"quit")==0)	{cout<<"I am stopping"<<endl; break;}
		buf.pid = getpid();
		if(-1==msgsnd(msgid,&buf,sizeof(buf),IPC_NOWAIT))	perror("Send2");
	}

	msgctl(msgid,IPC_RMID,0);
	pthread_join(rec,NULL);
	return 0;
}
