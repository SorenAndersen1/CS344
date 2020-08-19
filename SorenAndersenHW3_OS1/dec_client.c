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
//THIS FILE IS LITERALLY THE EXACT SAME AS ENC_CLIENT EXCEPT WITH A FEW DIFFERENT NUMBERS (SAME COMMENTS I COPIED AND PASTED)
        /*------------------------------------------------- charCheck -----
         |  Function charCheck
         |
         |  Purpose:  This takes in the c strings to check them for illegal chars
		 | according to the instructions this needs to take place on key and plaintext
         |
         |  Parameters: encrString = plaintext, key = key text
         |
         |  Returns:  Nothing, but can exit out of the program
         *-------------------------------------------------------------------*/

void charCheck(char* encrString, char* key){
	int i = 0;
	for (i = 0; i < strlen(encrString); i++)
	{
		if((encrString[i] > 64 && encrString[i] < 91) || encrString[i] == ' '){ //if chars are A-Z or SPACE
			continue;

		}
		else if(encrString[i] < 64 || encrString[i] > 91){
			fprintf(stderr, "Bad File Text In Encryption String File\n" ); //error if otherwise

			exit(1);
		}
	}
	int j = 0;
	for (j = 0; j < strlen(key); j++)
	{
				if((key[j] > 64 && key[j] < 91) || key[j] == ' '){ //same comparison 
			continue;

		}
		else if(key[j] < 64 || key[j] > 91){
			fprintf(stderr, "Bad File Text In key File\n" ); //different error

			exit(1);
		}
	}
	if(i > j){
			fprintf(stderr, "Key File is shorther than Encryption String s\n" ); //if the two strings arent the same size
			exit(1);
	}

}

        /*------------------------------------------------- openFile -----
         |  Function openFile
         |
         |  Purpose: opens a file to read, then copies it content into strings
         |
         |  Parameters:
         |      file = file to strip information
         | 		strLength = int pointer to get the size of the newly created string
         |		encrString = newly made string from contents of file
         |
         |  Returns:  No Return
         *-------------------------------------------------------------------*/
void openFile(FILE* file, int* strLength, char* encrString){
	int i = 0;
	int j = 0;
	char c;

   for (c = getc(file); c != EOF; c = getc(file))  //getc to to get char 
  
        *strLength = *strLength + 1;  //simple finding the length of file first


    fseek(file, 0, SEEK_SET); //set the file pointer back to the first spot

	for (i = 0; i < *strLength; i++)
	{
		c = fgetc(file); //actually creating the string
		if(c == EOF || c == '\n'){ //if they are these characters we are done
			break;
		}
		encrString[i] = c; //making the string (each char)
	}

	encrString[*strLength - 1] = '\0'; //eh im scared that it will overrun
	fclose(file); //close the file we had open


}

        /*------------------------------------------------- serverSetup -----
         |  Function serverSetup
         |
         |  Purpose:  Sets up the socket, to be connected to the server
         |
         |  Parameters:
         |      port = port number given at command line
         |
         |  Returns: Returns fully connected socket
         *-------------------------------------------------------------------*/
int serverSetup(int port){
	struct sockaddr_in serverFull; //simple materials setup
	struct hostent* serverJustHost;

	serverJustHost = gethostbyname("localhost");
	serverFull.sin_port = htons(port);
	serverFull.sin_family = AF_INET;
	memcpy((char*)&serverFull.sin_addr.s_addr, (char*)serverJustHost->h_addr, serverJustHost->h_length); //this code is straight from
	//enc_server if you want to read more about the process


	int socketFull = socket(AF_INET, SOCK_STREAM, 0); //setup socket

	if(socketFull < 0){
	fprintf(stderr, "Error Opening Socket"); //error handeling opening socket
				exit(2);

	}

	int returnSocket = 	connect(socketFull, (struct sockaddr*)&serverFull, sizeof(serverFull));
	if(returnSocket < 0){ //connect fully since this is the client and doesnt have to listen
		fprintf(stderr, "Error Connecting");
					exit(2);

	}

	return socketFull; //return socket connected

}





        /*------------------------------------------------- main -----
         |  Function main
         |
         |  Purpose:  Bad main as it does too much but it works.
         |  		  First calls functions to read in all the info (plaintext and keyfile), then does error handleing thru
         |			  calling the char check func, then sets up the socket connection with the port number from argv
         |			  then sends the plaintext, key, and type to enc_server only to wait for a recieved message back with the
         |			  ciphertext, then ends.
         |
         |  Parameters:
         |  	argc = number of command line arguments
         |		argv = command line arguments (expected: 4 total: enc_client plaintext_file key_file port_num)
         |
         |  Returns:  IF THIS FUNCTION SENDS BACK A VALUE VIA THE RETURN
         |      MECHANISM, DESCRIBE THE PURPOSE OF THAT VALUE HERE.
         *-------------------------------------------------------------------*/
int main(int argc, char **argv){



	if(argc > 4){
		printf("%s\n", "Try Again, input error");
		exit(1);
	}


	int i = 0;
	int memory = 0;
	int port = 0;
	int strLength = 0;
	int socketNum = 0;
	int keyLength = 0;

	char encrString[150000];
	char key[150000];



	if(argv[1] != NULL){
	FILE* fileName = fopen(argv[1], "r"); //open file 1
	if(fileName == NULL){
		printf("%s\n", argv[1]);
		//fprintf("%s\n", argv[2]);
		exit(0);
	}
	openFile(fileName, &strLength, encrString); //extract string


}
	if(argv[2] != NULL){
	FILE* fileName = fopen(argv[2], "r"); //open file 2
	if(fileName == NULL){
		printf("%s\n", argv[2]);
		//fprintf("%s\n", argv[2]);
		exit(0);
	}

	openFile(fileName, &keyLength, key); //extract string
}

	charCheck(encrString, key); //check both newly made strings for errors

	if(argv[3] != NULL)
		port = atoi(argv[3]);

	socketNum = serverSetup(port); //setup server

	int errorCheck = 0;
	int encrpt = 1;






	char buff[150000]; //setup string for sending
	// memset(buff, '\0', sizeof(buff));
	 char* typeSet = "2\n";

	// 	errorCheck = send(socketNum, typeSet, sizeof(typeSet), 0);
	// 	if(errorCheck < 0 ){
	// 		printf("error sending\n");
	// 		printf("%s\n", strerror(errno));
	// 	}
			memset(buff, '\0', sizeof(buff));
	sprintf(buff, "%s\n%s\n%s", key, encrString, typeSet); //simple sprintf to abuse






		errorCheck = send(socketNum, buff, sizeof(buff), 0); //error checking the send
		if(errorCheck < 0 ){
			fprintf(stderr, "%s\n", strerror(errno) );
			exit(2);
		}
			memset(buff, '\0', sizeof(buff));

		if(recv(socketNum, buff, sizeof(encrString) + 1, 0) < 0){ //recieving the encrypted text
			fprintf(stderr, "%s\n", strerror(errno) );
			exit(2);
					//exit(1);
				}
		fprintf(stdout, "%s\n", buff ); //give cipher text then we done

	close(socketNum);
	exit(0);
}

