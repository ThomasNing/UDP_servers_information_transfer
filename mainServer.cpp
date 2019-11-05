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
#define MYPORT 22153

using namespace std;

int main(){
    int ID, F_Size, C, L, Vp;
	int capacity,length,velocity;
    int db_server_fd, calc_server_fd;
    int len, sin_size;

    struct sockaddr_in main_addr;
    struct sockaddr_in db_addr, calc_addr;
    
    string buff;


    
    main_addr.sin_family = AF_INET;
    main_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    main_addr.sin_port = htons(MYPORT); 

    db_addr.sin_family = AF_INET;
    db_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    db_addr.sin_port = htons(23153); 
 



    cout << "The Main Server is up to running" << endl;
    while(1){

    while(1){
    stringstream oss;
    stringstream calcss;
    char message[1024] = {0};
    db_server_fd = socket(AF_INET,SOCK_DGRAM,0); 
    if (db_server_fd == -1) { 
        perror("socket error\n"); 
        return 1; 
    }

    sin_size = sizeof(db_addr);
    if(bind(db_server_fd,(struct sockaddr *)&main_addr,sizeof(main_addr))<0)
	{
		perror("bind error\n");
		return 1;
	}

    
        cout << "Please input link ID and file size(<ID> <size>): " << endl;
        cin >> ID >> F_Size;
        cout << "Link " << ID << ", file size " << F_Size << "MB." << endl;
        
        buff = to_string(ID);
        strcpy(message, buff.c_str());
        cout << "Send Link " << buff << " to database server." << endl;
        sendto(db_server_fd, message, 1024, MSG_CONFIRM, (struct sockaddr*)&db_addr, sin_size);
        recvfrom(db_server_fd, message, 1024, MSG_WAITALL, (struct sockaddr*)&db_addr, (socklen_t*)&sin_size);
        buff = message;
        //cout<<buff<<endl;
        oss.str(buff);
        oss >> buff;
        capacity = atoi(buff.c_str());
        //cout<<capacity<<endl;
        oss >> buff;
        length = atoi(buff.c_str());
        //cout<<length<<endl;
        oss >> buff;
        velocity = atoi(buff.c_str());
        //cout<<velocity<<endl;
       
       if (capacity==0&&length==0&&velocity==0){
        
        cout<<"No match found\n";
        close(db_server_fd);
        break;
        }
       else{
           cout << "Receive link capacity " << capacity << "Mbps, link length " << length << " km, propagation velocity " << velocity << "km/s." << endl;
       }
        
        oss.str(string());
        buff.clear();
        close(db_server_fd);
        //Now we start a different socket to do the transmission with calcServer
        C=capacity;
        L=length;
        Vp=velocity;

        calc_addr.sin_family = AF_INET;
        calc_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        calc_addr.sin_port = htons(24153); 
        calc_server_fd = socket(AF_INET,SOCK_DGRAM,0);
    if (calc_server_fd == -1) { 
        perror("socket error\n"); 
        return 1; 
    }

    if(bind(calc_server_fd,(struct sockaddr *)&main_addr,sizeof(main_addr))<0)
	{
		perror("bind error\n");
		return 1;
	}

        char messageNew[1024]={0};
        
   
        double Tr, Tp, Total;
        int size;
        cout << "Send information to calculation server." << endl;
        calcss << F_Size << " " << C << " " << L << " " << Vp;
        buff = calcss.str();
        strcpy(messageNew, buff.c_str());
        sendto(calc_server_fd, messageNew, 1024, MSG_CONFIRM, (struct sockaddr*)&calc_addr, sin_size);
        recvfrom(calc_server_fd, messageNew, 1024, MSG_WAITALL, (struct sockaddr*)&calc_addr, (socklen_t*)&sin_size);
        buff = messageNew;
        calcss.str(buff);
        calcss >> buff;
        Tr = atof(buff.c_str());
        calcss >> buff;
        Tp = atof(buff.c_str());
        calcss >> buff;
        Total = atof(buff.c_str());
        cout << "Receive transmission delay " << Tr << "ms, propagation delay " << Tp << "ms and total delay " << Total << "ms" << endl;

        


        oss.str(string());
        calcss.str(string());
        buff.clear();
        
        close(calc_server_fd);

    }
   }
    
    return 0;
}
