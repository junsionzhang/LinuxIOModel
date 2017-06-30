//客户端 
#include <stdio.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
 
int main(int argc, char* argv[])
{
    int socketfd, n;
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
     
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7779);
     
    connect(socketfd,(struct sockaddr *)  &serv_addr, sizeof(serv_addr));
     
    write(socketfd, "client message", 14);
    return 0;
 
}