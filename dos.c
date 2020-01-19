#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char* argv[]){

    if(argc < 2){
        printf("Wrong number of aurguments\n");
        exit(1);
    }

    int PORT = atoi(argv[1]);
    printf("%d\n",PORT);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket()");
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
    if(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("connect");
        exit(1);
    }
    printf("connected\n");
    write(sock, "GET / HTTP/1.1\r\n", strlen("GET / HTTP/1.1\r\n"));
    write(sock, "hello", strlen("hello"));
    close(sock);
    // sleep(86400);

    return 0;
}
