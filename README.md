## greyListing

[wikipedia](https://fr.wikipedia.org/wiki/Greylisting)  


## localhost

```sh
sol@debian:~/code/clientSMTP/SMTP$ ./clientSMTP3 bidon@homtila.com test mail.txt localhost sol.rosca@gmail.com 25
Trying localhost
Connected to localhost
Escape character is '^]'

Etat: START
220 debian.home ESMTP Exim 4.84_2 Sat, 17 Jun 2017 09:18:36 -0500

Etat: HELO
250 debian.home Hello localhost [127.0.0.1]

Etat: FROM
MAIL FROM: <bidon@homtila.com>
250 OK

Etat: TO
RCPT TO: <sol.rosca@gmail.com>
250 Accepted

Etat: DATA
354 Enter message, ending with "." on a line by itself

Etat: SUBJ
Subject: test

Etat: BODY
C'est pour ce soir!


Etat: DOT
250 OK id=1dMEYe-0000ZL-A1

Etat: QUIT
221 debian.home closing connection


fin
```

