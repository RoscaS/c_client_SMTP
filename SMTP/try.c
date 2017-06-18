#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

int main()
{
	printf("%s\n", system("cat log.txt | uniq -u | wc -l"));
}