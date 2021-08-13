#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
using namespace std;

pthread_t j;
vector <int> cli;

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

int main()
{
	int key = ftok(".",12345),msgid;
	msgid = msgget(key,0666);
	if(-1==msgid)	perror("make");
	int fl=1;
	while(fl)
		{
			if(-1==msgrcv(msgid,&buf,sizeof(buf),0,IPC_NOWAIT))	{sleep(3);}
			//perror("Receive");
			//else 
			//{	
				//cout<<buf.pid<<" "<<buf.mtext<<" "<<buf.mtype<<endl;
				if(buf.mtype == 100)
				{	int x=1;
					for(int j=0;j<cli.size();j++)
					{
						if(cli[j]==buf.pid)
						{
							x=0;break;
						}
					}
					if(x)
					{
						cli.push_back(buf.pid);
						cout<<"Pushed"<<endl;
					}
				}
				else if(buf.mtype == 101)
				{
					int p = buf.pid;
					buf.mtype = 102;
					for(int i=0;i<cli.size();i++)
					{
						if(strcmp(buf.mtext,"quit")==0)	{cout<<"I am stopping"<<endl; fl=0; break;}
						if(cli[i]!=p)
						{
							buf.pid=cli[i];
							cout<<"Sending: "<<buf.pid<<" Size: "<<cli.size()<<endl;
							if(-1==msgsnd(msgid,&buf,sizeof(buf),IPC_NOWAIT))	perror("Send");
						}
					}
				}
			//}	
			sleep(1);	
		}

	msgctl(msgid,IPC_RMID,0);
	//pthread_join(j,NULL);
	return 0;
}
