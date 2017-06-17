
## Local

```c
source      = "sol.rosca@he-arc.ch";
subject     = "test";
filePath    = "mail.txt";
domainDns   = "localhost";
destination = "claud.dup@hotmail.com";
portno      = PORT_SMTP; // 25
```


```sh
sol@debian:~/code/clientSMTP/01$ cd "/home/sol/code/clientSMTP/01/" && g++ clientSMTP.c -o clientSMTP && "/home/sol/code/clientSMTP/0
1/"clientSMTP
220 debian.home ESMTP Exim 4.84_2 Fri, 16 Jun 2017 18:25:12 -0500
HELO he-arc.ch
250 debian.home Hello localhost [127.0.0.1]
MAIL FROM: <sol.rosca@he-arc.ch>
250 OK
RCPT TO: <claud.dup@hotmail.com>
250 Accepted
DATA
354 Enter message, ending with "." on a line by itself
Subject: test
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do
eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut
enim ad minim veniam, quis nostrud exercitation ullamco laboris
nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore eu fugiat
nulla pariatur. Excepteur sint occaecat cupidatat non proident,
sunt in culpa qui officia deserunt mollit anim id est laborum.
.
250 OK id=1dM0c4-0000hp-CC
QUIT
221 debian.home closing connection
sol@debian:~/code/clientSMTP/01$
```


## smtp.alphanet.ch (25)

```c
source      = "sol.rosca@he-arc.ch";
subject     = "test";
filePath    = "mail.txt";
domainDns   = "smtp.alphanet.ch";
destination = "schaefer@alphanet.ch";
portno      = PORT_SMTP; // 25
```

```sh
sol@debian:~/code/clientSMTP/01$ cd "/home/sol/code/clientSMTP/01/" && g++ clientSMTP.c -o clientSMTP && "/home/sol/code/clientSMTP/0
1/"clientSMTP
220 shakotay.alphanet.ch ESMTP Postfix (Debian/GNU)
HELO he-arc.ch
250 shakotay.alphanet.ch
MAIL FROM: <sol.rosca@he-arc.ch>
250 2.1.0 Ok
RCPT TO: <schaefer@alphanet.ch>
554 5.7.1 Service unavailable; Client host [213.55.176.143] blocked using cbl.abuseat.org; Blocked - see http://www.abuseat.org/looku
p.cgi?ip=213.55.176.143
DATA
554 5.5.1 Error: no valid recipients
Subject: test
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do
eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut
enim ad minim veniam, quis nostrud exercitation ullamco laboris
nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore eu fugiat
nulla pariatur. Excepteur sint occaecat cupidatat non proident,
sunt in culpa qui officia deserunt mollit anim id est laborum.
.
221 2.7.0 Error: I can break rules, too. Goodbye.
QUIT
221 2.7.0 Error: I can break rules, too. Goodbye.
```

* err 5xx: refus totals


## smtp.alphanet.ch (587)

```c
source      = "sol.rosca@he-arc.ch";
subject     = "test";
filePath    = "mail.txt";
domainDns   = "smtp.alphanet.ch";
destination = "test@alphanet.ch";
portno      = PORT_SMTP;
```


```sh
sol@debian:~/code/clientSMTP/01$ cd "/home/sol/code/clientSMTP/01/" && g++ clientSMTP.c -o clientSMTP && "/home/sol/code/clientSMTP/0
1/"clientSMTP
220 shakotay.alphanet.ch ESMTP Postfix (Debian/GNU)
HELO he-arc.ch
250 shakotay.alphanet.ch
MAIL FROM: <sol.rosca@he-arc.ch>
250 2.1.0 Ok
RCPT TO: <test@alphanet.ch>
554 5.7.1 Service unavailable; Client host [213.55.176.143] blocked using cbl.abuseat.org; Blocked - see http://www.abuseat.org/looku
p.cgi?ip=213.55.176.143
DATA
554 5.5.1 Error: no valid recipients
Subject: test
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do
eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut
enim ad minim veniam, quis nostrud exercitation ullamco laboris
nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in
reprehenderit in voluptate velit esse cillum dolore eu fugiat
nulla pariatur. Excepteur sint occaecat cupidatat non proident,
sunt in culpa qui officia deserunt mollit anim id est laborum.
.
221 2.7.0 Error: I can break rules, too. Goodbye.
QUIT
221 2.7.0 Error: I can break rules, too. Goodbye.
```