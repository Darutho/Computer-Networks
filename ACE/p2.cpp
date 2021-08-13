#include <ace/Reactor.h>
//#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Synch.h>
#include <ace/Log_Msg.h>
#include <ace/streams.h>
#include <ace/INET_Addr.h>
#include <bits/stdc++.h>
#include <errno.h>
using namespace std;
int main()
{
ACE_INET_Addr server("127.0.0.1:8001");
ACE_INET_Addr client(9000,"127.0.0.1");
ACE_SOCK_Dgram client_data(client);
 
char message[40];
size_t sent_data_length = client_data.recv(message, 
                                              40, server);
if (sent_data_length == -1) 
  printf("Error in data transmission\n");
else
{
cout<<message<<endl;
}

 
client_data.close();
return 0;
}
