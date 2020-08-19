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

/*
https://www.poftut.com/what-is-sleep-function-and-how-to-use-it-in-c-program/
https://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
https://stackoverflow.com/questions/503878/how-to-know-what-the-errno-means
http://www.asciitable.com/
https://pubs.opengroup.org/onlinepubs/007908799/xns/syssocket.h.html
*/
//ALL CODE BLOCK STYLING STOLEN FROM https://www2.cs.arizona.edu/~mccann/styletemplatesCP.html


/*------------------------------------------------- stripMsg -----
         |  Function stripMsg
         |
         |  Purpose:  Simple token stripping, when the full message is sent
         | 	this splits it into the string, key, and type
         |
         |  Parameters: full_Enc = Full sent string from enc_client, outputEnc = Stripped plaintext
         | outputKey = Stripped Key,  type = Stripped type
         |      
         |
         |  Returns: None
         *-------------------------------------------------------------------*/
void stripMsg(char* full_Enc, char* outputEnc, char* outputKey, char* type){
	//printf("size of full %d\n", strlen(full_Enc) );

	char* stripTemp = strtok(full_Enc, "\n");
	//printf("size of string %d\n", strlen(stripTemp) );
	strcpy(outputKey, stripTemp);
	stripTemp = strtok(NULL, "\n");
	//	printf(" SIZE OF KEY %d\n", strlen(stripTemp) );

	strcpy(outputEnc, stripTemp);
	stripTemp = strtok(NULL, "\n");
	if(stripTemp != NULL){
	strcpy(type, stripTemp);
		//	printf(" SIZE OF size %d\n", strlen(stripTemp) );
}
}

/*------------------------------------------------- encr -----
         |  Function encr
         |
         |  Purpose:  Deals with encryption solely, just simple addition really
         |
         |  Parameters:
         |     plaintext = encryption message, key = encryption key, messageSize = message length
         |  returnval = where we want encrypted message to go
         |
         |  Returns:  None
         *-------------------------------------------------------------------*/
void encr(char* plaintext, char* key, int messageSize, char* returnval){
	int i = 0;
	int quickMath = 0;






	for (i = 0; i < messageSize; i++)
	{

		if(plaintext[i] != '\n'){
			if(plaintext[i] == ' '){
				plaintext[i] = 26; //26 = final alphalet letter (0-26) = 27 total
			}
			else{
				plaintext[i] = plaintext[i] - 65; //make it 0-26
			}

		}
		
			if(key[i] == ' '){
				key[i] = 26; //again change the text string with spaces to something more mathmatic
			}
			else{
				key[i] = key[i] - 65;
			
		}
		returnval[i] = ((key[i] + plaintext[i]) % 27); //add the two nums together than mod 27 to get encrpyt

			if(returnval[i] == 26){
				returnval[i] = ' '; //if its 26 then a space
				//otherwise just make it a captial letter based upon what 0-26 value it got
			}
			else{
				returnval[i] = returnval[i] + 65;
			
		}


	}
		returnval[messageSize] = '\0'; //learned this from last project, always good to make new strings end with \0
}

/*------------------------------------------------- seperate -----
         |  Function seperate
         |
         |  Purpose:  Most of the prog, once everything is set up it forks off
         | then recieves the data from the socket, then calls stripping func, then encr, then finally
         | sends all the data back to the client nessecary, then closes off.
         |
         |  Parameters:
         |      connection = fully connected socket , messageSize = size of encr message, keySize = size of key, 
         |		crp = where encrypted message will go
         |
         |  Returns:  IF THIS FUNCTION SENDS BACK A VALUE VIA THE RETURN
         |      MECHANISM, DESCRIBE THE PURPOSE OF THAT VALUE HERE.
         *-------------------------------------------------------------------*/
void seperate(int connection, int *messageSize, int *keySize, char* crp){

	pid_t newPid = -1;
	int i = 0;

	int status = -1;

	newPid = fork();

	if(newPid == -1){
		fprintf(stderr, "Error on Child\n" ); //classic setup
	}


	if(newPid == 0){ //child
	char finalPlaintext[150000];  //plaintext3 = 1024
		memset(finalPlaintext, '\0', sizeof(finalPlaintext));
	char key[150000];  //plaintext3 = 1024
		memset(key, '\0', sizeof(key));
	char encrString[150000]; //simple setup of value
		memset(encrString, '\0', sizeof(encrString));
	char buff[150000]; //as in slides
	memset(buff, '\0', sizeof(buff));

	char* returnval = malloc(sizeof(char) * *messageSize); //i love C malloc is so fun
	





				if(recv(connection, buff, sizeof(buff) + 1, 0) < 0){ //recieving first message (full)
		fprintf(stderr, "%s\n", strerror(errno));


					//exit(1);
				}

			strcpy(crp, buff); //copy it into plaintext spot
							// printf("%d\n", strlen(crp));

				char type[3]; //setup for type comparison

				stripMsg(crp, finalPlaintext, key, type); //strip the message for all the individual values
							// printf("%d\n", strlen(finalPlaintext));
							// printf("%d\n", strlen(key));

					int temp = strlen(finalPlaintext); //simple fix for message size
					//printf("%d\n", sizeof(finalPlaintext));
					if(type[0] != '1' && (strlen(finalPlaintext) < 60000)){
						fprintf(stderr, "Connected to Wrong Server try Again\n");
						exit(2); //this is where I determine if the socket is connected to the right type of prog (enc v dec)
					}
				encr(finalPlaintext, key, temp, encrString); //do the actual encryption value changing

		int errorCheck = 0; //set up for send

		errorCheck = send(connection, encrString, strlen(encrString), 0); //send just encrypted value back, nothing else
		if(errorCheck < 0 ){
		fprintf(stderr, "%s\n", strerror(errno));
		}

				close(connection); //close connection
				exit(0); //weve done it

	}

	else{ //parent

		close(connection); //close their verison of connection
		pid_t classic = waitpid(-1, &status, WNOHANG); //just relax until the child is done
	}

}

/*------------------------------------------------- serverSetup -----
         |  Function serverSetup
         |
         |  Purpose:  Sets up all socket related things for the server
         |
         |  Parameters: port = port number given at command line
		 |
         |
         |  Returns:  full active connected listening socket
         *-------------------------------------------------------------------*/
int serverSetup(int port){
	struct sockaddr_in serverFull;
	struct hostent* serverJustHost;

	serverJustHost = gethostbyname("localhost"); //i wonder what it will be
	serverFull.sin_port = htons(port); //little endian
	serverFull.sin_addr.s_addr = INADDR_ANY; //simple socket handeling
	serverFull.sin_family = AF_INET;
	memcpy((char*)&serverFull.sin_addr.s_addr, (char*)serverJustHost->h_addr, serverJustHost->h_length); //thank god for in class material





	int socketFull = socket(AF_INET, SOCK_STREAM, 0); //set up intial socket
	
	if(socketFull < 0){
		fprintf(stderr, "%s\n", strerror(errno));
	exit(2);
	}

	if(bind(socketFull, (struct sockaddr *)&serverFull, sizeof(serverFull)) < 0){ //bind the intial socket to port and connection
		fprintf(stderr, "%s\n", strerror(errno));
		exit(2);

	}
	listen(socketFull, 5); //wait for someone to show up

	return socketFull; //return the newly made connection
}


/*------------------------------------------------- main -----
         |  Function main
         |
         |  Purpose:  Takes in command line inputs, then calls some intial setup functions
         | Finally it just loops around waiting for the next connection
         |
         |  Parameters:
         |      argc = number of command line arguments
         |      argv = command line arguments expected to be "enc_server PORT_NUMBER"
         |
         |  Returns:  A good ole time
         *-------------------------------------------------------------------*/
int main(int argc, char **argv){



	if(argc > 3){
		fprintf(stderr, "%s\n", strerror(errno)); //if there are any more arguments its wrong
		exit(1); //since the max it can be is "enc_server PORT_NUMBER &" which is 3
	}
	socklen_t clientLen; //https://pubs.opengroup.org/onlinepubs/007908799/xns/syssocket.h.html
	struct sockaddr_in clientNeed;

	int port = 0;
	int strLength = 0;
	int socketNum = 0;
	char encrString[150000];

	char finalKey[150000];
	char finalPlaintext[150000];

	if(argv[1] != NULL)
		port = atoi(argv[1]); //convert port number

	int serverNum = serverSetup(port); //setup server with newly converted port number

	clientLen = sizeof(clientNeed); //in class materia;s
	int socketComplete = -1;	
		


while(1){
		socketComplete = accept(serverNum, (struct sockaddr *)&clientNeed, &clientLen); //accept next connection




	if(socketComplete < 0){
		fprintf(stderr, "%s\n", strerror(errno)); //error handeling for next connection

	}
		// char buff[50];
		// memset(buff, '\0', sizeof(buff));
		// char* compare = "enc\n";
		// 		if(recv(socketComplete, buff, sizeof(buff), 0) < 0){
		// 						printf("%s\n", strerror(errno));

		// 			//exit(1);
		// 		}
		// 		printf("NUM RECIEVED: %s\n", buff );
		// 		if(strcmp(buff, compare) != 0){
		// 					fprintf(stderr, "Error on Connection To Wrong Prog\n" );

		// 			exit(1);
		// 		}
		// 		else{
		// 			printf("SUCCESS \n");
		// 			memset(buff, '\0', sizeof(buff));
		// 		}




	int messageSize = 0;
	int stringSize = 0;

	seperate(socketComplete, &messageSize, &stringSize, encrString); //go in to fork part of prog
}



	

	exit(0);
}

