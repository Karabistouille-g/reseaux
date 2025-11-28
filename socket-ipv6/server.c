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

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in6 serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_port = htons(atoi(argv[1]));
    serv_addr.sin6_addr = in6addr_any;

    int binding = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (binding == -1) {
        perror("bind");
        close(sock);
        return 1;
    }

    int listening = listen(sock, 5);
    if (listening == -1) {
        perror("listen");
        close(sock);
        return 1;
    }

    fprintf(stdout, "Server is listening");

    int connect = accept(sock, NULL, NULL);
    if (connect == -1) {
        perror("accept");
        close(sock);
        return 1;
    }
    
    char *msg = "Message du serveur";
    int sent = send(sock, msg, sizeof(msg), 0);
    if (sent == -1) {
        perror("send");
        close(sock);
        return 1;
    }

    fprintf(stdout, "%s\n", msg);
    close(sock);
    return 0;
}