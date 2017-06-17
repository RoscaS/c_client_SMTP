## todo:

* tests codes err + wireshark


## greyListing

[wikipedia](https://fr.wikipedia.org/wiki/Greylisting)  




## localhost (sol)

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


## smtp.alphanet.ch (sol)

```sh
Trying smtp.alphanet.ch
Connected to smtp.alphanet.ch
Escape character is '^]'

Etat: START
220 shakotay.alphanet.ch ESMTP Postfix (Debian/GNU)

Etat: HELO
250 shakotay.alphanet.ch

Etat: FROM
MAIL FROM: <sol.rosca@he-arc.ch>
250 2.1.0 Ok

Etat: TO
RCPT TO: <schaefer@alphanet.ch>
554 5.7.1 Service unavailable; Client host [213.55.176.143] blocked using cbl.abuseat.org; Blocked - see http://www.abuseat.org/lookup.cgi?ip=213.55.176.143

Etat: DATA
554 5.5.1 Error: no valid recipients

Etat: SUBJ
Subject: test

Etat: BODY
C'est pour ce soir!


Etat: DOT
221 2.7.0 Error: I can break rules, too. Goodbye.

Etat: QUIT
221 2.7.0 Error: I can break rules, too. Goodbye.


fin
```

## ## smtp.alphanet.ch (steven)

```sh
Trying smtp.alphanet.ch
Connected to smtp.alphanet.ch
Escape character is '^]'
Etat: START
220 shakotay.alphanet.ch ESMTP Postfix (Debian/GNU)
Etat: HELO
250 shakotay.alphanet.ch
Etat: FROM
MAIL FROM: <steven.jeanneret@he-arc.ch>
250 2.1.0 Ok
Etat: TO
RCPT TO: <schaefer@alphanet.ch>
450 4.2.0 <schaefer@alphanet.ch>: Recipient address rejected: Greylisted, see http://postgrey.schweikert.ch/help/alphanet.ch.html
Etat: DATA
554 5.5.1 Error: no valid recipients
Etat: SUBJ
Subject: test
Etat: BODY
C'est pour ce soir!
Etat: DOT
221 2.7.0 Error: I can break rules, too. Goodbye.
Etat: QUIT
221 2.7.0 Error: I can break rules, too. Goodbye.
fin
```
