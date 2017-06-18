#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>

#define PORT_SMTP 25 // SMTP: port 25, (587: auth, 465: ssl)
#define BUFFER_SIZE 1024

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

// machineEtat: on/off
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

     if(arg[6]!=NULL) {
        md->portno  = atoi(arg[6]);
     }
     else {
        md->portno  = PORT_SMTP;
     }
}

// prototypes
int  machineEtat(const MailData *args, const int sleepTime);
int  connexion(const MailData *md);
void finConnexion(const int sock);
void errExit(char *err, const int sock);


// =================================================
// ==========           Main              ========== 
// =================================================

int main(int argc, char **argv) {
    //check args
    if (argc < 6) {
        printf("ERROR bad-args\nusage:\t%s from subject filePath domainDNS to [port]\n", argv[0]);
        exit(1);
    }
    // declaration & initialisation objet MailData
    MailData md1;
    initMD(&md1, argv);
    // lancement machine d'etat
    machineEtat(&md1,0);
    return EXIT_SUCCESS;
}
// ========= End of Main ==========



// =================================================
// ==========           Connexion         ========== 
// =================================================

int connexion(const MailData *md) {
    // declaration des variables utiles
    int                sock;
    struct sockaddr_in servAddr;    
    struct hostent    *serv;        

    // creation socket
    if (!(sock = socket(AF_INET, SOCK_STREAM, 0))) {
        errExit("ERROR: error opening socket", sock);
    }

    // initialisation des attributs descripteurs socket
    if (!(serv = gethostbyname(md->domainDns))) {
        errExit("ERROR: no such host", sock);
    }

    bzero( ( char *) &servAddr, sizeof(servAddr));    
    servAddr.sin_family = AF_INET;

    bcopy( (char *)  serv->h_addr, 
           (char *) &servAddr.sin_addr.s_addr, 
                     serv->h_length);

    servAddr.sin_port = htons(md->portno);

    // connexion au serveur
    if (connect(sock, 
               (struct sockaddr*) &servAddr, 
               sizeof(servAddr)) < 0) {
        errExit("ERROR: error connecting", sock);
    }

    printf("\nConnected to %s\n", md->domainDns);
    return sock;
}

// ========= End of connexion ==========


// =================================================
// ========== connexionFin ==========         
// =================================================

void finConnexion(const int sock) {
    if (sock < 0) {
        return;
    }
    printf("Closing connexion...\n");
    close(sock);
}

// ========= End of connexionFin ==========


// =================================================
// ========== machineEtat ==========         
// =================================================

int machineEtat(const MailData *args, int sleepTime) {

	sleep(sleepTime);

    int  etat, sock, onOff, recup, pid;
    char buffer[BUFFER_SIZE];
    FILE *f   = NULL;
    FILE *txt = NULL;

    onOff = ON; 
    etat  = START;
    sock  = connexion(args);

    if (!(f = fdopen(sock, "r+"))) {
        errExit("ERROR: can't open socket", sock);
    }

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

                if (!(txt = fopen(args->filePath, "r"))) {           // DRY
                    errExit("ERROR: can't open mail file", sock);
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

            case ERROR4:
                printf("\nEtat: ERROR4\n");
                fprintf(stderr, "ERROR %c%c%c: grey-listed\n", buffer[0], buffer[1], buffer[2]);
                fprintf(stderr, "forking process & retry in 10'...");
                onOff = OFF;

                pid = fork();

                // process enfant
                if (pid == 0) {
                    printf("Child process: retrying to send in 10'...\n");
                    machineEtat(args, 30); // changer a 600 apres tests !
                }

                // process parent
                if (pid > 0) {
                    printf("Exit parent process..\n");
                    fclose(f);
                    finConnexion(sock);
                    exit(0);
                }

	            // echec forking
	            else {
                    errExit("ERROR: fork failed", sock);   
	            }

                break;     

            case ERROR5:
                printf("\nEtat: ERROR5\n");
                fprintf(stderr, "ERROR %c%c%c: final-error\n", buffer[0], buffer[1], buffer[2]);
                finConnexion(sock);
                fprintf(stderr, "exit...\n");
                exit(1);
                break;
            
            default:
                errExit("ERROR: illegal state", sock);       
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
    fclose(f);
    finConnexion(sock);
    return 0;
}

// ========= End of machineEtat ==========


// =================================================
// ========== errExit ==========         
// =================================================

void errExit(char *err, const int sock) {
    fprintf(stderr, "%s\n", err);
    finConnexion(sock);
    fprintf(stderr, "exit...\n");
    exit(-1);
}

// ========= End of errExit ==========


