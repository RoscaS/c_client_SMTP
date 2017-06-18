#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

// ATTENTION ! Gros soucis avec cette version du code.
// Fonctionnelle mais à compiler en C++ et non pas C.

#define PORT_SMTP 25 // SMTP: port 25, (587: auth, 465: ssl)
#define BUFFER_SIZE 1024

struct MailData {
    const char *source;
    const char *subject;
    const char *filePath;
    const char *domainDns;
    const char *destination;
          int   portno;

    void init(char **arg) {
        // source      = arg[1];
        // subject     = arg[2];
        // filePath    = arg[3];
        // domainDns   = arg[4];
        // destination = arg[5];
        source      = "sol.rosca@he-arc.ch";
        subject     = "test";
        filePath    = "mail.txt";
        domainDns   = "localhost";
        destination = "sol.rosca@gmail.com";
        portno      = PORT_SMTP;
    }
};



void finConnexion(int sock);
int connexion(const MailData *data);
int echange(const MailData *data);

void envoie(int sock, char *buff);
int reponse(int sock);



int main(int argc, char **argv) {
    // if (argc < 6) {
    //     fprintf(stderr, "\nusage %s hostname port\n\n", argv[0]);
    //     exit(0);
    // }
    

    MailData m1;
    m1.init(argv);

    // si la valeur de retour = 1, cela veut dire que le message 
    // s'est fait grey liste (err 4xx). On re-test apres 10 minutes
    if (echange(&m1)) {
        sleep(600);     // implementer version avec fork
        echange(&m1);
    }
    return 0;    
}



void finConnexion(int sock) {
    close(sock);
}



int echange(const MailData *data) {
    int  sock, code, grayFlag;
    char buffer[BUFFER_SIZE];
    const char *dom = NULL;
    FILE *f;
    sock = connexion(data);
    code = reponse(sock);
    dom  = strchr(data->source, '@');
    bzero(buffer,BUFFER_SIZE);

    // optionnel mais fonctionnel !
    if (dom == NULL) {
        perror("ERROR domain parsing"); // checker les messages d'err (ERRORxxxxx:succes en cas d'err)
        exit(1);
    }

    ++ dom; // pointe sur le premier char du nom de dom

    // initialisation 
    // sprintf(buffer, "HELO %s", dom);
    sprintf(buffer, "HELO host");
    envoie(sock, buffer);
    code = reponse(sock);
    bzero(buffer,BUFFER_SIZE);

    // envoie SOURCE
    sprintf(buffer, "MAIL FROM: <%s>", data->source);
    envoie(sock, buffer);
    code = reponse(sock);
    bzero(buffer,BUFFER_SIZE);

    // envoie DESTINATION
    sprintf(buffer, "RCPT TO: <%s>", data->destination);
    envoie(sock, buffer);
    code = reponse(sock);
    // 4xx => msg grey listed
    if (code >= 400 && code < 500) {
        grayFlag = 1;
    }
    grayFlag = 0;
    bzero(buffer,BUFFER_SIZE);

    // envoie DATA
    sprintf(buffer, "DATA");
    envoie(sock, buffer);
    code = reponse(sock);
    bzero(buffer,BUFFER_SIZE);

    // envoi SUJET
    sprintf(buffer, "Subject: %s", data->subject);
    envoie(sock, buffer);
    bzero(buffer,BUFFER_SIZE);

    // envoie DATA contenu
    f = fopen(data->filePath, "r");
    if (f < 0) {
        perror("ERROR can't open file");
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), f)) {
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        envoie(sock, buffer);
        bzero(buffer,BUFFER_SIZE);
    }
    fclose(f);

    // envoi POINT
    sprintf(buffer, ".");
    envoie(sock, buffer);
    code = reponse(sock);
    bzero(buffer,BUFFER_SIZE);



    // envoi QUIT
    sprintf(buffer, "QUIT");
    envoie(sock, buffer);
    code = reponse(sock);
    bzero(buffer,BUFFER_SIZE);

    // fin de l'echange
    finConnexion(sock);
    return grayFlag;
}



// retourne le code msg SMTP
int reponse(int sock) {
    char buffer[BUFFER_SIZE];
    int  buff_idx = 0;

    do {
        read(sock, buffer + buff_idx, 1);
    } 
    while (buffer[buff_idx++] != '\n' && buff_idx < sizeof(buffer));

    buffer[buff_idx-1] = '\0';
    printf("%s\n", buffer);
    return atoi(buffer); 
}



void envoie(int sock, char *buff) {
    printf("%s\n", buff);
    write(sock, buff, strlen(buff));
    write(sock, "\r\n", 2);
}



int connexion(const MailData *data) {
    // declaration des variables utiles
    int                socket_desc;
    struct sockaddr_in servAddr;
    struct hostent    *serv;

    // creation socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // initialisation des attributs des structures socket
    serv = gethostbyname(data->domainDns);
    if (serv == NULL) {
        perror("ERROR, no such host\n");
        exit(1);
    }

    bzero( ( char *) &servAddr, sizeof(servAddr));    
    servAddr.sin_family = AF_INET;

    bcopy( (char *)  serv->h_addr, 
           (char *) &servAddr.sin_addr.s_addr, 
                     serv->h_length);

    servAddr.sin_port = htons(data->portno);

    // connexion au serveur
    if (connect(socket_desc, 
               (struct sockaddr*) &servAddr, 
               sizeof(servAddr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    return socket_desc;
}