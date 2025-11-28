#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main (int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s @serv port [nomInterfSortante]\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in6 serv_addr;
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(atoi(argv[2]));
    
    int inet = inet_pton(AF_INET6, argv[1], &serv_addr.sin6_addr);
    if (inet <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    if (argc == 4) {
        int index = if_nametoindex(argv[3]);
        if (index == 0) {
            perror("if_nametoindex");
            close(sock);
            return 1;
        }
        serv_addr.sin6_scope_id = index;
    }

    int co = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (co == -1) {
        perror("connect");
        close(sock);
        return 1;
    }

    char msg[500];
    int sent = recv(sock, msg, sizeof(msg), 0);
    if (sent == -1) {
        perror("recv");
        close(sock);
        return 1;
    }

    fprintf(stdout, "%s\n", msg);
    close(sock);
    return 0;
}