#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#define SERVPORT 80
#define BACKLOG 10 // max numbef of client connection
#define MAXDATASIZE 100 

int main(char argc, char *argv[])
{
    int sockfd, client_fd, addr_size, recvbytes;
    char rcv_buf[MAXDATASIZE], snd_buf[MAXDATASIZE];
    char* val;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int bReuseaddr = 1; 

    char IPdotdec[20]; 

    /* create a new socket and regiter it to os .
    * SOCK_STREAM means that supply tcp service, 
    * and must connect() before data transfort.
    */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket:");
        exit(1);
    } 

    /* setting server's socket */
    server_addr.sin_family = AF_INET;         // IPv4 network protocol
    server_addr.sin_port = htons(SERVPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // auto IP detect
    memset(&(server_addr.sin_zero),0, 8);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(int));
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))== -1)
    {
        perror("bind:");
        exit(1);
    } 

    /* 
    * watting for connection , 
    * and server permit to recive the requestion from sockfd 
    */
    if (listen(sockfd, BACKLOG) == -1) // BACKLOG assign thd max number of connection
    {
        perror("listen:");
        exit(1);                                                                 
    }                                                                          

    while(1)                                                                   
    {                                                                          
        addr_size = sizeof(struct sockaddr_in);                                  

        /*                                                                       
        * accept the sockfd's connection,                                       
        * return an new socket and assign far host to client_addr               
        */                                                                      
        printf("watting for connect...\n");                                      
        if ((client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size)) == -1)   
        {                                                                        
            /* Nonblocking mode */                                                 
            perror("accept:");                                                     
            continue;                                                              
        }                                                                        

        /* network-digital to ip address */                                      
        inet_ntop(AF_INET, (void*)&client_addr, IPdotdec, 16);                   
        printf("connetion from:%d : %s\n",client_addr.sin_addr, IPdotdec);       

        //if (!fork())                                                           
        {                                                                        
        /* child process handle with the client connection */                  

        /* recive the client's data by client_fd */                            
        if ((recvbytes = recv(client_fd, rcv_buf, MAXDATASIZE, 0)) == -1)      
        {                                                                      
            perror("recv:");                                                     
            exit(1);                                                             
        }                                                                      
        rcv_buf[recvbytes]='\0';                                               
        printf("recv:%s\n", rcv_buf);    

        *snd_buf='\0';                                                         
        strcat(snd_buf, "welcome");                                            

        sleep(3);                                                              
        /* send the message to far-hosts by client_fd */                       
        if (send(client_fd, snd_buf, strlen(snd_buf), 0) == -1)                
        {                                                                      
            perror("send:");                                                     
            exit(1);                                                             
        }                                                                      
        printf("send:%s\n", snd_buf);                                          

        close(client_fd);                                                      
        //exit(1);                                                             
        }                                                                        

        //close(client_fd);                                                      
    } 

    return 0;                                                                  
}