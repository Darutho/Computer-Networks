#include <ace/Reactor.h>
#include <ace/SOCK_Acceptor.h>
//#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Dgram.h>
//#include <ace/Synch.h>
//#include <ace/Log_Msg.h>
//#include <ace/streams.h>
#include <ace/INET_Addr.h>
#include <bits/stdc++.h>
#include <errno.h>
#include<bits/stdc++.h>
#include "ace/OS_main.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/OS_NS_string.h"
#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_unistd.h"
using namespace std;
int main()
{
ACE_INET_Addr server("127.0.0.1:8001"); 
//ACE_INET_Addr server(458);
ACE_SOCK_Dgram server_data(server);
ACE_INET_Addr client(9000,"127.0.0.1");
char message[]="Message from server\n"; 
size_t sent_data_length = server_data.send(message,strlen(message) + 1, client);
if (sent_data_length == -1) 
  printf("Error in data transmission\n");
else
{
	cout<<"Sent!\n";
}
 
server_data.close();
return 0;
}
