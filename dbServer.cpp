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
#include <fstream>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#define MYPORT 23153
using namespace std;

int main(){
    int client_fd;
    int ID;
	int len, sin_size;
	struct sockaddr_in main_addr;
    struct sockaddr_in db_addr;
	char message[1024] = {0};
    
    string buff;

	memset(&main_addr,0,sizeof(main_addr));

	main_addr.sin_family=AF_INET;
	main_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	main_addr.sin_port=htons(22153);

    db_addr.sin_family = AF_INET;
    db_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    db_addr.sin_port = htons(23153); 
    client_fd = socket(AF_INET,SOCK_DGRAM,0); 
    if (client_fd == -1) { 
        perror("socket error\n"); 
        return 1; 
    }

    sin_size = sizeof(main_addr);
    if(bind(client_fd,(struct sockaddr *)&db_addr,sizeof(main_addr))<0)
	{
		perror("bind error\n");
		return 1;
	}

    while(1){




    cout << "The Database Server is up and running" << endl;
    buff = "No";
    strcpy(message, buff.c_str());
    

    recvfrom(client_fd, message, 1024, MSG_WAITALL, (struct sockaddr*)&main_addr, (socklen_t*)&sin_size);
    cout << "Receive request from Main Server." << endl;
    
    ID = atoi(message);
    
    //k is the total lines of the txt file
    int k=0;
    string line;
    ifstream original("database.txt");
    for (k=0;getline(original, line);++k);
    original.close();

    int code[100];
    int generator[100];
    int generator1[100];
    int generator2[100];
    ifstream input;
    int buf;
    input.open("database.txt");
    int j = 0;
    int counter=0;
    string oneLine;
    

    
        for(int i=0; i<k; i++){
            
            getline(input,oneLine);
            stringstream ss;
            ss.str(oneLine);
            ss>>code[counter]>>generator[counter]>>generator1[counter]>>generator2[counter];
            
            
        if(code[counter] == ID)
        {
           buff="Yes"; 
            
            break;
        }
        counter=counter+1;
       
        }
    
    input.close();
    
        if (buff=="No"){
         cout << "No match found" << endl;
         buff = "Recieve no match found";
         strcpy(message, buff.c_str());
        }
        else{
        cout << "Send link " << code[counter] << ", capacity " << generator[counter] << "Mbps, link length" << generator1[counter] << "km, propagation velocity " << generator2[counter] << "km/s." << endl;
        buff = to_string(generator[counter])+ " " + to_string(generator1[counter]) + " " + to_string(generator2[counter]);
        strcpy(message, buff.c_str());
        
        }

    sendto(client_fd, message, 1024, 0, (struct sockaddr*)&main_addr, sin_size);
    buff="restart";
    }
    close(client_fd);
    return 0;
}
