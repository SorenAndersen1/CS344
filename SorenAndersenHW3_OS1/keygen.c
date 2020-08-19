#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <time.h>



int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "%s many arguments, try again\n" );
		exit(1);
	}
	int fullKey; //rand var

	srand(time(0)); //setting seed

	int len = atoi(argv[1]); //take num in

	int i = 0;
	char alphabet[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};
	//array of alphabet plus space
//days later I have now realized I couldve just down "ABCDEF..." 
	for (i = 0; i < len; i++)
	{
		fullKey = rand() % 27; //choose a random number

			fprintf(stdout, "%c", alphabet[fullKey]); //assign that number's corresponding letter
	}
	fprintf(stdout, "\n");//last one has instrucstions say
}