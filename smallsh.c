#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>


int fore = 0; //lord forgive for the sins I have committed

void statusFunc(int* status){
	if(WIFEXITED(*status)){
		printf("exit value %d\n", WEXITSTATUS(*status));
		fflush(stdout);
	}
	else if(WIFSIGNALED(*status)){
		printf("Terminated by signal %d\n", WTERMSIG(*status) );
		fflush(stdout);
	}
	// else if(WIFSIGNALED(*status) == 0 && WIFEXITED(*status) == 0){
	// 	printf("exit value %d\n", 0);
	// }
}

void sysCommands(char** argsList,  char* inputFile, char* outputFile, int argCount, struct sigaction new_action, int fileBool, int groundProperty, char* commandFix){
	int inFile, outFile, secondCheckIn, outputCheckin;
	int status_num = 0;
	pid_t child = -1;
	child = fork();

	if(child == -1){
		fprintf(stderr, "Trouble Creating Fork\n" );
		exit(1);
	}

	if(child == 0){
	if(inputFile != NULL && fileBool == 2){
		inFile = open(inputFile, O_RDONLY);
		if(inFile == -1){
			printf("Input File Cant open\n");
			exit(1);
		}
		secondCheckIn = dup2(inFile, 0);
		if(secondCheckIn == -1){
			printf("Input File Cant dup\n");
			exit(2);

		}
	}

	if(outputFile != NULL && fileBool == 1){
		outFile = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(outFile == -1){
			printf("output File Cant open\n");
			exit(1);
		}
		outputCheckin = dup2(outFile, 1);
		if(outputCheckin == -1){
			printf("Output File Cant dup\n");
			exit(2);

		}
	}
	else{
		outFile = ("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	// int i = 0;
	// char** list;
	// if(argCount == 1){
	// 	list = malloc(sizeof(char*) * 2);
	// 	strcpy(list[0], argsList[0]);
	// 	list[1] = NULL;
	// }
	// else if(argCount > 1){
	// 	list = malloc(sizeof(char*) * argCount - 1);
	

	// for (i = 1; i < argCount; i++)
	// {
	// 	strcpy(list[i - 1], argsList[i]);
	// }
	// }

	argsList[argCount] = NULL;
	if(strcmp(commandFix, "ls") == 0){
		argsList[1] == NULL;
	}
	// int i = 0;
	// for (i = 0; i < 512; i++)
	// {
	// 	args
	// }
	if(execvp(commandFix, argsList) < 0){
		printf("Command Failure\n");
		fflush(stdout);
		exit(1);
	}
	
}
	else if(child > 0){
		do{
		waitpid(child, &status_num, 0);
	// 		if(WIFEXITED(status_num)){
	// 	printf("exit value %d\n", WEXITSTATUS(status_num));
	// 	fflush(stdout);
	// }
	// else if(WIFSIGNALED(status_num)){
	// 	printf("Terminated by signal %d\n", WTERMSIG(status_num) );
	// 	fflush(stdout);
	// }
		}while(!WIFEXITED(status_num) && !WIFSIGNALED(status_num));
	}

	// pid_t child = -1;
	// child = fork();

	// printf("%d\n", fileBool );
	// printf("child: %d\n", child );

	// if(child == -1){
	// 	perror("Your command really messed me up");
	// }
	// else if(child == 0){
	// 	//child
	// 	fflush(stdout);
	// 	if(fore == 1){
	// 		new_action.sa_handler = SIG_DFL;
	// 		sigaction(SIGINT, &new_action, NULL);
	// 	}

	// 	if(fileBool == 1){
	// 		printf("opening output file\n");
	// 		int openFile = open(outputFile, O_WRONLY);
	// 			if(openFile == -1){
	// 				perror("Error Opening File\n");
	// 				exit(1);
	// 			}
	// 			else{
	// 			int wrFile = dup2(openFile, 1);
	// 			}
	// 			close(openFile);
	// 	}
	// 	else if(fileBool == 2){
	// 						printf("opening input file\n");

	// 			int openUpFil = open(inputFile, O_RDONLY);
	// 			if(openUpFil == -1){
	// 				perror("Error Opening File\n");
	// 				exit(1);
	// 			}
	// 			else{
	// 			int rdFile = dup2(openUpFil, 0);
	// 				}
	// 			close(openUpFil);

	// 	}

	// 	else if(fileBool == -1){
	// 		printf("About to run: %s\n", argsList[0]);
	// 		// freopen("/dev/null", "w", stdout);
	// 		// freopen("/dev/null", "r", stdin);
	// 		// fcloseall();
	// 	}

	// 	if(fileBool != 1){

	// 	}



	// 	if(execvp(argsList[0], argsList) < 0 && fileBool == -1){
	// 		//perror(argsList[0]);
	// 		printf("command error\n");
	// 		exit(1);
	// 	}
	
	// }
	// else if(child >= 1){
	// 	//parent
	// 	if(groundProperty == 0){
	// 		printf("background pid is %d\n", child);
	// 	}
	// 	else{
	// 	//	waitpid(child, )
	// 	}
	// }


}



int childEzCommands(char* commandFix,  char* inputFile, char* outputFile, int argCount, int* status, int* ezCheck, int groundProperty, char** argsList){

	*ezCheck = 0;

	if(strcmp(commandFix, "exit") == 0){
	*ezCheck = 1;
		return 1;
	} 
	else if (strcmp(commandFix, "cd") == 0 )
	{
		//printf("MADE IT \n" );

		if (argCount == 1)
		{
			chdir(getenv("HOME"));
		}

			else if(argsList[1] != NULL){

		//	printf("GOING TO DIRECTORY:  %s\n", argsList[1]);
			if(chdir(argsList[1]) == -1){
				printf("Couldnt Find Directory\n");
			}




		}
	*ezCheck = 1;

	}
	else if(strcmp(commandFix, "status") == 0){
		statusFunc(status);
	*ezCheck = 1;

	}
	return 0;
}


int specialArgs(char** argsList,  char* inputFile, char* outputFile, int argCount, int* fileBool){
	int i = 0;
	int j = 0;
	char* tempstring = "%d";
	*fileBool = -1;
	for (i = 0; i < argCount; i++)
	{
		for (j = 0; j < strlen(argsList[i]); j++)
		{
			
			if (argsList[i][j] ==  '$' && argsList[i][j + 1] ==  '$') //right here i would love to do arglist[][i] but it isnt legal
			{
				sprintf(argsList[i], tempstring, getpid());
				printf(argsList[i]);
				break;
			}
			else if(argsList[i][j] == '>'){
			//	printf("got here\n" );
				strcpy(outputFile, argsList[i + 1]);
				strcpy(argsList[i], outputFile);
				*fileBool = 1;
				//printf("%s\n", outputFile );
				break;
			}
			else if(argsList[i][j] == '<'){
				strcpy(inputFile, argsList[i + 1]);
				strcpy(argsList[i], inputFile);

				*fileBool = 2;
				break;
			}
			else if(argsList[i][j] == '&' && fore == 0){
				argsList[i][j] = ' ';
				return 0;
			}
		}
	}
	// if(*fileBool == 1 && *fileBool == 2){
	// 	for (i = 1; i < argCount; i++)
	// 	{
	// 		argsList[i] = NULL;
	// 	}
	// }
	return 1;
}

int argsHelper(char** argsList){
	int i = 0;
	int totalArgCount = 0;
	char tempInput[2048];
	

	printf("%s", ": "); //print command line
	fflush(stdout); //good practice
	fgets(tempInput, 2048, stdin);
	strtok(tempInput, "\n");
	char* strip = strtok(tempInput, " ");// need to start off getting each indivdual command
	//This is where pointers come in to be useful;


	for (i = 0; i < 512; i++)
	{
		if (strip == NULL)
		{
			break;
		}
		else if(strip != NULL){

			argsList[i] = strdup(strip);
			strip = strtok(NULL, " "); //move to next
			totalArgCount++;

		}
	}


	return totalArgCount;

}




void SIGFunc(int signum){

	if(fore == 0){
		fore = 1;
		char* temp_for_message = "Entering foreground-only mode (& is now ignored)\n";
		write(1, temp_for_message, 49);
	//	char* short_message = ": "; //command again
	//	write(1, short_message, 2);
		fflush(stdout);

	}
	else if(fore == 1){
		fore = 0;
		char* temp_for_message = "Exiting foreground-only mode\n";
		write(1, temp_for_message, 29);
	//	char* short_message = ": ";
	//	write(1, short_message, 2);
		fflush(stdout);
	}

}

int main(){
	char** args = malloc(512 * sizeof(char*));
	int i = 0;
	// for (i = 0; i < 512; ++i)
	// {
	// 	args[i] = "";
	// }
	int totalArgs = 0;
	int groundProperty = -1; //0 == foreground, 1 = background 
	int exitcheat = 0;
	char* inputFile = malloc(sizeof(char) * 50);
	char* outputFile = malloc(sizeof(char) * 50);
	int status = -1;
	int ezCheck = -1;
	int fileBool = -1;

	//https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
	struct sigaction new_action = {0};
	struct sigaction old_action = {0};


	//https://man7.org/linux/man-pages/man2/sigaction.2.html
	old_action.sa_handler = SIG_IGN; //https://en.cppreference.com/w/c/program/SIG_strategies
	sigaction(SIGINT, &old_action, NULL);

	new_action.sa_handler = SIGFunc; //freaks me out its not SIGFunc()
	sigaction(SIGTSTP, &new_action, NULL);



	while(1){
		groundProperty = 1;
		totalArgs = argsHelper(args);
				fileBool = -1;


		groundProperty = specialArgs(args, inputFile, outputFile, totalArgs, &fileBool);

		char** newList;
		newList = malloc(totalArgs * sizeof(char*));
		char commandFix[20];
		strcpy(commandFix, args[0]);

		// for (i = 1; i < totalArgs; i++)
		// {
		// 	strcpy(newList[i - 1], args[i]);
		// }
		// if(newList[0] == NULL){
		// 	strcpy(newList[0], commandFix); 
		// }
		//printf("%s\n", args[]);

		if(args[0][0] != '#'){
					for (i = 0; i < totalArgs; i++)
		{
			//printf("%s\n", args[i] );
		}


			exitcheat = childEzCommands(commandFix, inputFile, outputFile, totalArgs, &status, &ezCheck, groundProperty, args);
			if (exitcheat == 1)
			{
				exit(0);
			}
			//printf("%d\n", ezCheck );
			//printf("%d\n", fileBool);
			if(!ezCheck){
			sysCommands(args, inputFile, outputFile, totalArgs, new_action, fileBool, groundProperty, commandFix);
		}


		for (i = 0; i < 512; i++)
		{
			args[i] == "";
		}


		}
	//	printf("%d\n", groundProperty );
		//printf("%d\n", totalArgs );


	}
}
