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
    const char *sujet;
    const char *filePath;
    const char *domainDns;
    const char *destination;
          int   portno;
};

// initialisation des parametres
void initMD(MailData *md, char **arg) {
     md->source      = arg[1];
     md->sujet       = arg[2];
     md->filePath    = arg[3];
     md->domainDns   = arg[4];
     md->destination = arg[5];
     if(arg[6]!=NULL)
     {
        //atoi convertis un string en int
        md->portno  = atoi(arg[6]);
     }
     else  
     {
        md->portno  = PORT_SMTP;
     }
    //md->source      = "sol.rosca@he-arc.ch";
    //md->sujet       = "test";
    //md->filePath    = "mail.txt";
    //md->domainDns   = "localhost";
    //md->destination = "sol.rosca@gmail.com";
    //md->portno      = PORT_SMTP;
}

// prototypes
int machineEtat(const MailData *args);
int  connexion(const MailData *md);
void finConnexion(const int sock);
// void envoie(const int sock, char *buff);
// int  reponse(const int sock);
// int routine(const int sock, char *buff);
int erreurs(FILE *f, char code);

int main(int argc, char **argv) {
     // check args
     if (argc < 6) {
         //fprintf(stderr, "\nusage %s \n\n", argv[0]);
        printf("Paramètres manquants !\n");
        printf("Source sujet file domain DNS destination [port] \n");
         exit(1); // todo: fonction help
     }

    MailData md1;

    initMD(&md1, argv);


    machineEtat(&md1);
    
    return 0;
}

int connexion(const MailData *md) {
    // declaration des variables utiles
    printf("Trying %s\n", md->domainDns);

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
    printf("Connected to %s\n", md->domainDns);
    printf("Escape character is '^]'\n");
    return socket_desc;
}


void finConnexion(const int sock) {
    close(sock);
}


// // retourne le code msg SMTP
// int reponse(const int sock) {
//     char buffer[BUFFER_SIZE];
//     int  buff_idx = 0;

//     do {
//         read(sock, buffer + buff_idx, 1);
//     } 
//     while (buffer[buff_idx++] != '\n' && buff_idx < sizeof(buffer));

//     buffer[buff_idx-1] = '\0';
//     printf("%s\n", buffer);
//     return atoi(buffer); 
// }



// void envoie(const int sock, char *buff) {
//     printf("%s\n", buff);
//     write(sock, buff, strlen(buff));
//     write(sock, "\r\n", 2);
// }


// int routine(const int sock, char *buff) {
//     int code;
//     envoie(sock, buff);
//     code = reponse(sock);
//     bzero(buff, BUFFER_SIZE);
//     return code;
// }


int machineEtat(const MailData *args) {
    int etat, sock, onOff;
    // int code;
    // char code_char[3];
    // int grayFlag;
    char buffer[BUFFER_SIZE];
    FILE *f, *txt;

    onOff = ON; 
    etat  = START;
    sock  = connexion(args);
    f     = fdopen(sock, "r+");

    while(onOff) {
        int recup = 0;
        switch(etat) {

            case START:
                printf("\nEtat: START\n");
                break;

            case HELO:
                printf("\nEtat: HELO\n");
                fprintf(f, "HELO host\r\n");
                break;

            case FROM:
                printf("\nEtat: FROM\n");
                fprintf(f,"MAIL FROM: <%s>\r\n",args->source);
                printf("MAIL FROM: <%s>\r\n",args->source);
                break;

            case TO:
                printf("\nEtat: TO\n");
                fprintf(f,"RCPT TO: <%s>\r\n",args->destination);
                printf("RCPT TO: <%s>\r\n",args->destination);
                break;

            case DATA:
                printf("\nEtat: DATA\n");
                fprintf(f,"DATA\r\n");
                break;

            case SUBJ:
                printf("\nEtat: SUBJ\n");
                fprintf(f,"Subject: %s\r\n",args->sujet);
                printf("Subject: %s\r\n",args->sujet);
                recup =1;
                break;

            case BODY:
                printf("\nEtat: BODY\n");

                txt = fopen(args->filePath, "r");

                if (txt < 0) {
                    perror("ERROR can't open file");
                    exit(1);
                }
                while (fgets(buffer, sizeof(buffer), txt)) {
                    fputs(buffer,f);
                    printf("%s\n",buffer );
                }
                fclose(txt);
                recup=1;
                break;

            case DOT:
                printf("\nEtat: DOT\n");
                fprintf(f,"\r\n.\r\n");
                break;

            case QUIT:
                printf("\nEtat: QUIT\n");
                fprintf(f,"QUIT\r\n");
                onOff = OFF;                                
                break;

            case ERROR:
                printf("\nEtat: ERROR\n");
                break;
            
            default:
                exit(1);        
        }
        if(recup==0)
        {
            fgets(buffer, sizeof(buffer), f);
            printf("%s", buffer);
        }
        etat++;        


        

        


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

int erreurs(FILE *f, char code) {
    char buffer[1024];
    fgets(buffer, sizeof(buffer), f);
    if(buffer[0]==code) {
	    return 0;
    }
    else {
	    switch (buffer[0]) {
	        case '2':
                printf("\n %d, recu : 2\n",code);
                return -1;
	        case '3':
                printf("\n %d, recu : 3\n",code);
                return -1;
	        case '4':
                printf("\n %d, recu : 3\n",code);
                return 1;
	        case '5':
               	perror("\npas traite, recu : 5 \n");
		        return -1;
	        default:
               	perror("\nERR: pas entre 2 et 5 \n");
		        return -1;
    	}
    }
}
