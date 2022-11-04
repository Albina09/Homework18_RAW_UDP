/*UDP c AF_INIT*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <linux/udp.h>
#include <linux/ip.h>

void errorExit(char err[]);

struct udpHeader{
    short int source;
    short int dest;
    short int len; 
    short int check;
};

void errorExit(char err[]){
    perror(err);
    exit(EXIT_FAILURE);
}


int main(){
    
    struct sockaddr_in serv;
    char msg[] = "Hello, server";
    char buff[40];

    struct udpHeader header;

   
    //----------------------------------------------------
    int fd = socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
    if(fd == -1)
        errorExit("soket");

    serv.sin_family = AF_INET;
    serv.sin_port = htons(9006);
    serv.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    //------------------------------------------

    header.source = htons(6666);
    header.dest = htons(9006);
    header.len = htons(sizeof(msg) + sizeof(struct udpHeader));
    header.check = 0;


    memcpy((void *)buff, (void* )&header,sizeof(header));
    memcpy((void*)(buff+sizeof(header)),(void*)msg,sizeof(msg));
   
    
    socklen_t size = sizeof(serv);
    if(sendto(fd, buff, (sizeof(struct udpHeader) + sizeof(msg)), 0, (struct sockaddr *)&serv, sizeof(serv)) == -1)
        errorExit("sendto");

    char out[100];
    while(1){
        if(recvfrom(fd, out, sizeof(out), 0, (struct sockaddr *)&serv, &size) == -1)
            errorExit("recvfrom");
        printf("%s\n", out + 28);
    }

    exit(EXIT_SUCCESS);

}