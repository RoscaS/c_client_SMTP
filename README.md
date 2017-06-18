## todo:

* tests codes err + wireshark


https://ssl.horus.ch/~schaefer/bin/view/HEArc/DirectivesClientSMTP

## greyListing

[wikipedia](https://fr.wikipedia.org/wiki/Greylisting)  


## smtp.alphanet.ch (fork test) (wiresharkForkGreylist.png)

```sh
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ ./clientSMTP3 nathan.latino@he-arc.ch test mail.txt smtp.alphanet.ch schaefer@alphanet.ch
Trying smtp.alphanet.ch
Connected to smtp.alphanet.ch
Escape character is '^]'

Etat: START
220 shakotay.alphanet.ch ESMTP Postfix (Debian/GNU)

Etat: HELO
250 shakotay.alphanet.ch

Etat: FROM
MAIL FROM: <nathan.latino@he-arc.ch>
250 2.1.0 Ok

Etat: TO
RCPT TO: <schaefer@alphanet.ch>
450 4.2.0 <schaefer@alphanet.ch>: Recipient address rejected: Greylisted, see http://postgrey.schweikert.ch/help/alphanet.ch.html

Etat: ERROR4
ERROR 450: grey-listed
forking process & retry in 10'...Exit parent process..
forking process & retry in 10'...Child process: retrying to send in 10'...
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ ls
total 28
-rwxr-xr-x 1 sol sol 12008 Jun 18 10:15 clientSMTP3
-rwxr-xr-x 1 sol sol  6886 Jun 18 10:15 clientSMTP3.c
-rwxr-xr-x 1 sol sol    20 Jun 18 09:23 mail.txt
-rwxr-xr-x 1 sol sol    84 Jun 18 09:23 makefile
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ Trying smtp.alphanet.ch
Connected to smtp.alphanet.ch
Escape character is '^]'

Etat: START
220 shakotay.alphanet.ch ESMTP Postfix (Debian/GNU)

Etat: HELO
250 shakotay.alphanet.ch

Etat: FROM
MAIL FROM: <nathan.latino@he-arc.ch>
250 2.1.0 Ok

Etat: TO
RCPT TO: <schaefer@alphanet.ch>
450 4.2.0 <schaefer@alphanet.ch>: Recipient address rejected: Greylisted, see http://postgrey.schweikert.ch/help/alphanet.ch.html

Etat: ERROR4
ERROR 450: grey-listed
forking process & retry in 10'...Exit parent process..
forking process & retry in 10'...Child process: retrying to send in 10'...
```


## localhost

```sh
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ ./clientSMTP3 un@deux.com test mail.txt localhost un@trois.com
Trying localhost
Connected to localhost
Escape character is '^]'

Etat: START
220 debian.home ESMTP Exim 4.84_2 Sun, 18 Jun 2017 10:32:02 -0500

Etat: HELO
250 debian.home Hello localhost [127.0.0.1]

Etat: FROM
MAIL FROM: <un@deux.com>
250 OK

Etat: TO
RCPT TO: <un@trois.com>
250 Accepted

Etat: DATA
354 Enter message, ending with "." on a line by itself

Etat: SUBJ
Subject: test

Etat: BODY
C'est pour ce soir!


Etat: DOT
250 OK id=1dMcBG-000179-FR

Etat: QUIT
221 debian.home closing connection
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ 
```

## smtprel.he-arc.ch (test reel)

```sh
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ ./clientSMTP3 sol.rosca@he-arc.ch test mail.txt smtprel.he-arc.ch steven.jeanneret@outlook.com 
Trying smtprel.he-arc.ch
Connected to smtprel.he-arc.ch
Escape character is '^]'

Etat: START
220 ***************************************************

Etat: HELO
250 srv-smtpapp1.he-arc.ch

Etat: FROM
MAIL FROM: <sol.rosca@he-arc.ch>
250 2.1.0 Ok

Etat: TO
RCPT TO: <steven.jeanneret@outlook.com>
250 2.1.5 Ok

Etat: DATA
354 End data with <CR><LF>.<CR><LF>

Etat: SUBJ
Subject: test

Etat: BODY
C'est pour ce soir!


Etat: DOT
250 2.0.0 Ok: queued as 1F45B60DC2

Etat: QUIT
221 2.0.0 Bye
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ 
```



## Err 500

```sh
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ ./clientSMTP3 sol.rosca@gmail test mail.txt aspmx.l.google.com test@testt.ch
Trying aspmx.l.google.com
Connected to aspmx.l.google.com
Escape character is '^]'

Etat: START
220 mx.google.com ESMTP h32si6380560edc.376 - gsmtp

Etat: HELO
250 mx.google.com at your service

Etat: FROM
MAIL FROM: <sol.rosca@gmail>
250 2.1.0 OK h32si6380560edc.376 - gsmtp

Etat: TO
RCPT TO: <test@testt.ch>
550-5.1.1 The email account that you tried to reach does not exist. Please try

Etat: ERROR5
ERROR 550: final error
exit...
sol@debian:~/code/clientSMTP/SMTP/clientSMTP$ 
```

