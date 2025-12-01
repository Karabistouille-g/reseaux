#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main (int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    
    int ip = inet_pton(AF_INET, argv[1], &addr.sin_addr);
    if (ip == 0) {
        perror("inet_aton");
        return 1;
    }

    int co = connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    if (co == -1) {
        perror("connect");
        return 1;
    }

    char buf[500];
    int msg = recv(sock, buf, 500, 0);
    if (msg == -1) {
        perror("recv");
        return 1;
    } else if (msg == 0) {
        fprintf(stderr, "Disconnect\n");
        return 1;
    }

    buf[msg] = '\0';
    fprintf(stdout, "%s\n", buf);

    int shut = shutdown(sock, SHUT_RDWR);
    if (shut == -1) {
        perror("shut");
        return 1;
    }

    close(sock);

    return 0;
}