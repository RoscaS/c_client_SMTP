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
#define ERROR4 9
#define ERROR5 10

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
    //  md->source      = arg[1];
    //  md->sujet       = arg[2];
    //  md->filePath    = arg[3];
    //  md->domainDns   = arg[4];
    //  md->destination = arg[5];
    //  if(arg[6]!=NULL)
    //  {
    //     //atoi convertis un string en int
    //     md->portno  = atoi(arg[6]);
    //  }
    //  else  
    //  {
    //     md->portno  = PORT_SMTP;
    //  }
    md->source      = "quentin.michel@he-arc.ch";
    md->sujet       = "test";
    md->filePath    = "mail.txt";
    md->domainDns   = "smtp.alphanet.ch";
    md->destination = "schaefer@alphanet.ch";
    md->portno      = PORT_SMTP;
}

// prototypes
int machineEtat(const MailData *args, int sleepTime);
int  connexion(const MailData *md);
void finConnexion(const int sock);
int erreurs(FILE *f, char code);
void childProcess(int sock);

int main(int argc, char **argv) {
     // check args
    //  if (argc < 6) {
    //      //fprintf(stderr, "\nusage %s \n\n", argv[0]);
    //     printf("Paramètres manquants !\n");
    //     printf("Source sujet file domain DNS destination [port] \n");
    //      exit(1); // todo: fonction help
    //  }

    MailData md1;

    initMD(&md1, argv);


    machineEtat(&md1, 0);
    
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
    // shutdown(sock, 2);
    close(sock);
}


int machineEtat(const MailData *args, int sleepTime) {
    sleep(sleepTime);
    int etat, sock, onOff, recup, pid;
    char buffer[BUFFER_SIZE];
    FILE *f, *txt;

    onOff = ON; 
    etat  = START;
    sock  = connexion(args);
    f     = fdopen(sock, "r+");

    while(onOff) {
        recup = 0;
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
                fprintf(f,"Succes\r\n");
                //recup=1;
                onOff = OFF;                                
                break;

            case ERROR4:
                printf("\nEtat: ERROR4\n");
                printf("ERROR %c%c%c: grey-listed\n", buffer[0], buffer[1], buffer[2]);
                printf("forking process & retry in 10'...");
                onOff = OFF;
                // forking
                pid = fork();

                if (pid < 0) {
                    perror("ERROR on fork");
                    exit(1);
                }

                if (pid == 0) {
                    finConnexion(sock);
                    printf("Mail will be send in 10 minutes !\n");
                    printf("Exit...\n");
                    machineEtat(args, 600);
                    exit(0);
                }
                break;     

            case ERROR5:
                printf("\nEtat: ERROR5\n");
                printf("ERROR %c%c%c: final error\n", buffer[0], buffer[1], buffer[2]);
                printf("exit...\n");
                finConnexion(sock);
                exit(1);
                break;
            
            default:
                exit(1);        
        }

        if(recup == 0) {
            fgets(buffer, sizeof(buffer), f);
            printf("%s", buffer);   
        }
        if(buffer[0] == '4') {
            etat = ERROR4;
        }
        else if(buffer[0] == '5') {
            etat = ERROR5;
        }  
        else {
            etat++;        
        }
    }

    printf("\n\nfin\n\n");
    finConnexion(sock);

    return 0;    
}




