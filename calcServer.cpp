#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#define MYPORT 24153
using namespace std;

int main(){
    int client_fd;
    int Size, C, L, Vp;
	double Tr, Tp, Total;
	int len, sin_size;
	struct sockaddr_in main_addr, calc_addr;
	
    

	memset(&main_addr,0,sizeof(main_addr));

	main_addr.sin_family=AF_INET;
	main_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	main_addr.sin_port=htons(22153);

    calc_addr.sin_family = AF_INET;
    calc_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    calc_addr.sin_port = htons(24153); 
    client_fd = socket(AF_INET,SOCK_DGRAM,0); 
    if (client_fd == -1) { 
        perror("socket error\n"); 
        return 1; 
    }

    sin_size = sizeof(main_addr);
    if(bind(client_fd,(struct sockaddr *)&calc_addr,sizeof(main_addr))<0)
	{
		perror("bind error\n");
		return 1;
	}

    cout << "The Calculation Server is up and running" << endl;
	
    while(1){
    char message[1024] = {0};
    string buff;
    stringstream ss;
    
	buff = " ";
    strcpy(message, buff.c_str());
    

    recvfrom(client_fd, message, 1024, MSG_WAITALL, (struct sockaddr*)&main_addr, (socklen_t*)&sin_size);
    cout << "Receive request from Main Server." << endl;
	buff = message;
	ss << buff;
	ss >> Size >> C >> L >> Vp;
    cout<<Size<<C<<L<<Vp;
	
	Tr = 8*1000*Size/(double)C;
	Tp = 1000*L/(double)Vp;
	Total = Tp + Tr;

    buff=to_string(Tr)+" "+to_string(Tp)+" "+to_string(Total);
	strcpy(message, buff.c_str());
	sendto(client_fd, message, 1024, 0, (struct sockaddr*)&main_addr, sizeof(main_addr));
	cout << "Send transmission delay " << Tr << "ms, propagation delay " << Tp << "ms and total delay " << Total << "ms" << endl;
    }
	close(client_fd);
    return 0;
}