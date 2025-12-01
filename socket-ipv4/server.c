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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = INADDR_ANY;

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

    int connect = accept(sock, NULL, NULL);
    if (connect == -1) {
        perror("accept");
        close(sock);
        return 1;
    }
    
    char *msg = "Message du serveur";
    int sent = send(connect, msg, strlen(msg), 0);
    if (sent == -1) {
        perror("send");
        close(sock);
        return 1;
    }

    int shut = shutdown(sock, SHUT_RDWR);
    if (shut == -1) {
        perror("shut");
        return 1;
    }

    close(sock);

    return 0;
}