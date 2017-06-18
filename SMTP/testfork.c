#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>      // strlen
#include<arpa/inet.h>   // inet_addr
#include<unistd.h>      // write

void childProcess(int sock);

int main (int argc, char **argv) {
    int socket_desc;
    int new_socket;
    int n, pid;
    char buffer[256];
    struct sockaddr_in server;
    struct sockaddr_in client;

    // creation du socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        perror("ERROR opening socket");
        exit(1);
    }

    // initialisation des membres de l'objet server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // bind
    if (bind(socket_desc, (struct sockaddr * )&server, sizeof(server)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
    printf("\nbind done\n");

    // sleep mode en attendant un client
    listen(socket_desc, 3);
    printf("\nwaiting for incoming connections...\n");

    n = sizeof(struct sockaddr_in);

    // boucle infinie qui accepte et fork les clients distant
    while(1) {
        new_socket = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &n);

        if (new_socket < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        // affiche les infos du client
        printf("Connection from %s:%d accepted\n", 
            inet_ntoa(client.sin_addr), // conversion adr en un format Inet Std dot Ntation (str)
            ntohs(client.sin_port)); // conversion Network Byte Order vers Host Byte Order (endianess)
            
        // creation process enfant
        pid = fork();

        if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0) {
            // processus du client
            close(socket_desc);
            childProcess(new_socket);
            exit(0);
        }
        else {
            close(new_socket);
        }
    }
    return 0;
}

void childProcess(int sock) {
    int n;
    char buffer[256];
    bzero(buffer,256);

    // lecture du message du client
    n = read(sock, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from socket\n");
        exit(1);
    }
    // affichage message client
    printf("Message from client: %s\n", buffer);

    // ecrit une reponse au client
    n = write(sock, "ok", 18);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
}

