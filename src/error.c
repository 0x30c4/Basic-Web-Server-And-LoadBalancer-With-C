#include<error.h>

/*
 * All the fprintf functions are writing to STDERR.
 */

int check(int exp, const char *msg){
	if (exp == SOCKETERROR){
		fprintf(stderr, "%s%s%s", RED, UNDERLINE, BOLD);
		perror(msg);
		fprintf(stderr, "%s", RESETALL);
		exit(1);
	}
	return exp;
}


