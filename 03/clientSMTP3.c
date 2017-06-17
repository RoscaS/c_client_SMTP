#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define PORT_SMTP 25 // SMTP: port 25, (587: auth, 465: ssl)

// etats
#define START 0
#define HELO 1
#define FROM 2
#define TO   3
#define DATA 4
#define SUBJ 5
#define BODY 6
#define DOT  7
#define QUIT 8
#define ERROR 9

// on/off
#define OFF 0
#define ON  1

// struct parametres
typedef struct MailData MailData;
struct MailData {
    const char *source;
    const char *subject;
    const char *filePath;
    const char *domainDns;
    const char *destination;
          int   portno;
};

// initialisation des parametres
void initMD(MailData *md, char **arg) {
    // md->source      = arg[1];
    // md->subject     = arg[2];
    // md->filePath    = arg[3];
    // md->domainDns   = arg[4];
    // md->destination = arg[5];
    md->source      = "sol.rosca@he-arc.ch";
    md->subject     = "test";
    md->filePath    = "mail.txt";
    md->domainDns   = "localhost";
    md->destination = "sol.rosca@gmail.com";
    md->portno      = PORT_SMTP;
}

// prototypes
int machineEtat(const MailData *args);
int  connexion(const MailData *md);
void finConnexion(const int sock);
void envoie(const int sock, char *buff);
int  reponse(const int sock);
char* routine(const int sock, char *buff);

int main(int argc, char **argv) {
    // // check args
    // if (argc < 6) {
    //     fprintf(stderr, "\nusage %s \n\n", argv[0]);
    //     exit(1); // todo: fonction help
    // }

    MailData md1;

    initMD(&md1, argv);


    machineEtat(&md1);
    
    return 0;
}

int connexion(const MailData *md) {
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
    serv = gethostbyname(md->domainDns);
    if (serv == NULL) {
        perror("ERROR, no such host\n");
        exit(1);
    }

    bzero( ( char *) &servAddr, sizeof(servAddr));    
    servAddr.sin_family = AF_INET;

    bcopy( (char *)  serv->h_addr, 
           (char *) &servAddr.sin_addr.s_addr, 
                     serv->h_length);

    servAddr.sin_port = htons(md->portno);

    // connexion au serveur
    if (connect(socket_desc, 
               (struct sockaddr*) &servAddr, 
               sizeof(servAddr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    return socket_desc;
}


void finConnexion(const int sock) {
    close(sock);
}


// retourne le code msg SMTP
int reponse(const int sock) {
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



void envoie(const int sock, char *buff) {
    printf("%s\n", buff);
    write(sock, buff, strlen(buff));
    write(sock, "\r\n", 2);
}


char* routine(const int sock, char *buff) {
    int    code_int;
    char * code_str;
    envoie(sock, buff);
    code_int = reponse(sock);
    sprintf(code_str, "%d", code_int);
    bzero(buff, BUFFER_SIZE);
    return code_str;
}


int machineEtat(const MailData *args) {
    int etat, sock, onOff;
    char *code;
    // int grayFlag;
    char buffer[BUFFER_SIZE];
    // FILE *f;

    onOff = ON; 
    etat  = START;
    sock  = connexion(args);
    bzero(buffer, BUFFER_SIZE);

    while(onOff) {
        switch(etat) {

            case START:
                printf("\nEtat: START\n");
                break;

            case HELO:
                printf("\nEtat: HELO\n");
                sprintf(buffer, "HELO host");
                break;

            case FROM:
                printf("\nEtat: FROM\n");
                break;

            case TO:
                printf("\nEtat: TO\n");
                break;

            case DATA:
                printf("\nEtat: DATA\n");
                break;

            case SUBJ:
                printf("\nEtat: SUBJ\n");
                break;

            case BODY:
                printf("\nEtat: BODY\n");
                break;

            case DOT:
                printf("\nEtat: DOT\n");
                break;

            case QUIT:
                printf("\nEtat: QUIT\n");
                break;

            case ERROR:
                printf("\nEtat: ERROR\n");
                break;
            
            default:
                exit(1);        
        }



        code = routine(sock, buffer);

        printf("code: %s", code[0]);

        onOff = OFF;


        // fgets(buffer, sizeof(buffer), stdin);
        // buffer[strlen(buffer) - 1] = '\0'; // écrase le \n en fin de ligne
        // // voir doc pour comprendre le not.
        // if(!strcmp("q", buffer)) {
        //     printf("FIN AUTOMATE\n");
        //     return 0;
        // }
        // else if(!strcmp("n", buffer)) {
        //     // printf("FIN AUTOMATE\n");
        //     if (etat == QUIT) {
        //         printf("ERROR invalid input at this stage");
        //         exit(1);
        //     }
        //     ++etat;            
        // }
        // else if(!strcmp("s", buffer)) {
        //     etat = START;
        // }
        // else {
        //     printf("ERROR invalid input");
        // }
    }

    printf("\n\nfin\n\n");

    finConnexion(sock);

    return 0;    
}