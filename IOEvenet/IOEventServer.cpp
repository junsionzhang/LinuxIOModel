#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>  
#include <string.h> 
#include <unistd.h>  
#include <signal.h> 
#include <fcntl.h> 
 
int listenfd1;
 
void do_sigio(int sig)
{
    int clifd, clilen;
    struct sockaddr_in cli_addr;
    char buffer[256];

    clifd = accept(listenfd1, (struct sockaddr *) &cli_addr, &clilen);
    bzero(buffer, 256);
    read(clifd, buffer, 255);
    printf("Listenfd1 Message%s\r\n", buffer);
}
 
int main(int argc, char *argv[])
{
    //绑定监听7779端口的fd
    struct sockaddr_in serv_addr1;
    listenfd1 = socket(AF_INET, SOCK_DGRAM, 0);

    bzero((char *) &serv_addr1, sizeof(serv_addr1));
    serv_addr1.sin_family = AF_INET;
    serv_addr1.sin_port = htons(7779);
    serv_addr1.sin_addr.s_addr = INADDR_ANY;

    struct sigaction sigio_action;
    memset(&sigio_action, 0, sizeof(sigio_action));
    sigio_action.sa_flags = 0;
    sigio_action.sa_handler = do_sigio;
    sigaction(SIGIO, &sigio_action, NULL);

    fcntl(listenfd1, F_SETOWN, getpid());
    int flags;
    flags = fcntl(listenfd1, F_GETFL, 0);
    flags |= O_ASYNC | O_NONBLOCK;
    fcntl(listenfd1, F_SETFL, flags);

    bind(listenfd1, (struct sockaddr *) &serv_addr1, sizeof(serv_addr1));

    while(1);
    close(listenfd1);

    return 0; 
}