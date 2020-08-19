#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>



#define SIZE 10000 //10 * 1000 (aka max input in 5 seconds)

 char buffer1[SIZE]; //THIS IS THE BUFFER BETWEEN INPUT FUNC AND LINE SEPERATOR
 char buffer2[SIZE]; //THIS IS THE BUFFER BETWEEN LINE SEPERATOR AND PLUSSIGNFIX
 char buffer3[SIZE]; //THIS IS THE BUFFER BETWEEN PLUSSIGNFIX AND OUTPUT

 int buff1Size = 0; //producer cosumer check 1
 int buff2Size = 0; //producer consumer check 2
 int buff3Size = 0; //producer consumer check 3
//i set all the producer checks to see if the size is equal to 0, this is because I know
 //the consumers i set up for this specific secnario always set the length to something
 // I understand in a different secnario I might need a more accurate number to check for changes
 //but the canvas page just says it has to be a check of size, which mine is just for greater than 0

pthread_mutex_t firstBuff_mutex = PTHREAD_MUTEX_INITIALIZER; //mutex for buffer 1
pthread_mutex_t secondBuff_mutex = PTHREAD_MUTEX_INITIALIZER; //mutex for buffer 2
pthread_mutex_t thirdBuff_mutex = PTHREAD_MUTEX_INITIALIZER; //mutex for buffer 3



pthread_cond_t busy = PTHREAD_COND_INITIALIZER; //signal for 1
pthread_cond_t done = PTHREAD_COND_INITIALIZER; //signal for 1
pthread_cond_t busy1 = PTHREAD_COND_INITIALIZER; //signal for 2
pthread_cond_t done1 = PTHREAD_COND_INITIALIZER; //signal for 2
pthread_cond_t busy2 = PTHREAD_COND_INITIALIZER; //signal for 3
pthread_cond_t done2 = PTHREAD_COND_INITIALIZER; //signal for 3


void* output(void* num){
	char copyString[10000]; //String to Copy Buffer to
	char out[81]; //final output string
	int i = 0; //iterator
	int breakcmd = 0; //since I have so many nested loops figured a break BOOL would be easier
	char DONETEST[10000];
	int timesAround = 0;
	while(1){


		pthread_mutex_lock(&thirdBuff_mutex); //I WANT TO VIEW SOMETHING LETS LOCK IT
		while(buff3Size == 0) //CONSUMER CHECK FOR OUTPUT
		pthread_cond_wait(&done2, &thirdBuff_mutex); //wait for the signal if theres nothing in the buffer

		strcat(copyString, buffer3); //EASIEST WAY TO KEEP LESS THAN 80 CHARS IN THE OUTPUT STRING
		strcpy(DONETEST, buffer3); //need this for comparison for conditional
		
		buff3Size = 0; //ive taken something out theres nothing in the buffer
		memset(buffer3, '\0', strlen(buffer3)); //set it back to nothing 

		pthread_cond_signal(&busy2); //send signal to plussign
		pthread_mutex_unlock(&thirdBuff_mutex); //we are doing using the data


		if(DONETEST[0] == 'D' && DONETEST[1] == 'O'  && DONETEST[2] == 'N'  && DONETEST[3] == 'E' && DONETEST[4] == '\0'){
		 	breakcmd = 1; //check for a done command
		 }
		 if(DONETEST[0] == 'D' && DONETEST[1] == 'O'  && DONETEST[2] == 'N'  && DONETEST[3] == 'E' && DONETEST[4] == ' '){
		 	breakcmd = 1; //check for a done command
		 }

		while	(strlen(copyString) >= 80){ //while there are enough characters
			for (i = 0; i < 80; i++) //iterate over 80 chars
			{
				out[i] = copyString[i]; //copy buffer copy to output string
		if(copyString[i] == 'D' && copyString[i + 1] == 'O'  && copyString[i + 2] == 'N'  && copyString[i + 3] == 'E' && copyString[i + 4] == '\0'){
			if(!(copyString[i - 1] > 32 && copyString[i - 1] < 127)) //thisis cause I had some weird behavior on input 3 with the done
			breakcmd = 1; //another DONE check
		}
		if(copyString[i] == 'D' && copyString[i + 1] == 'O'  && copyString[i + 2] == 'N'  && copyString[i + 3] == 'E' && copyString[i + 4] == ' '){
			if(!(copyString[i - 1] > 32 && copyString[i - 1] < 127))//this is cause I had some weird behavior on input 3 with the done
			breakcmd = 1; //check for a done command
		}

			}
			if(breakcmd){
				//printf("ouput\n");
				break; //break loop
			}
			// if(timesAround == 3)
			// printf("BEFORE: \n %s\n", copyString);
			i = 0; 
			
				//printf("BEFORE: \n %s\n", copyString);
			
			while(copyString[i + 80] != '\0'){
				copyString[i] = copyString[i + 80]; //overwrite 80 characters just written out
				i++;
			}
			copyString[i] = '\0';
		
			 
			// printf("AFTER: \n %s\n", copyString);
			//printf("START\n");
			fprintf(stdout, "%s\n", out); //PRINT OUT 
			// printf("END\n");
			timesAround++;

			

		}
			if(breakcmd){
				memset(copyString, '\0', strlen(copyString)); //just in case
				memset(out, '\0', strlen(out));
				break; //final break
			}

	}
}
void* plusSignFix(void* num){

	char copyString[10000]; //copy for buffer 
	char output[10000]; //send to output
	int i = 0; 
	int iterator = 0;
	int donecmd = 0;
	while(1){
	i = 0; //reset both iterators
	iterator = 0;

		pthread_mutex_lock(&secondBuff_mutex); //lock
		while(buff2Size == 0) //CONSUMER CHECK PLUS SIGN
		pthread_cond_wait(&done1, &secondBuff_mutex); //wait for new if so
		
		strcpy(copyString, buffer2); //take the string

		buff2Size = 0; //ive taken something out
		memset(buffer2, '\0', strlen(buffer2)); //set it back to nothing 

		pthread_cond_signal(&busy1); //done taking the string
		pthread_mutex_unlock(&secondBuff_mutex); //unlock to give back access




	for (i = 0; i < 10000; i++)
	{

		if(copyString[iterator] == '\0')
			break; //if its the end of the string, its done
		else if(copyString[iterator] == '+' && copyString[iterator + 1] == '+'){ //if there is ++ change it with ^ and dont copy the second
			output[i] = '^';
			iterator = iterator + 1; //skipping over second

		} 
		else{
			output[i] = copyString[iterator]; //if its not anything special, just copy it over
		}
		if(copyString[i] == 'D' && copyString[i + 1] == 'O'  && copyString[i + 2] == 'N'  && copyString[i + 3] == 'E' && copyString[i + 4] == '\0'){
			if(!(copyString[i - 1] > 32 && copyString[i - 1] < 127)) //this is cause I had some weird behavior on input 3 with the done
			donecmd = 1; //another DONE check
		}
		if(copyString[i] == 'D' && copyString[i + 1] == 'O'  && copyString[i + 2] == 'N'  && copyString[i + 3] == 'E' && copyString[i + 4] == ' '){
			if(!(copyString[i - 1] > 32 && copyString[i - 1] < 127))
			donecmd = 1; //check for a done command
		}

		iterator++; //copied a char either way if we are here, lets keep going
	}
	memset(copyString, '\0', strlen(copyString)); //reset 


		pthread_mutex_lock(&thirdBuff_mutex);
		while(buff3Size != 0) //producer check for plus sign
		pthread_cond_wait(&busy2, &thirdBuff_mutex); //if theres something in there wait
		strcpy(buffer3, output); //send to output
		buff3Size = strlen(output); //also set length for check
		memset(output, '\0', strlen(output)); //reset 


		pthread_cond_signal(&done2);
		pthread_mutex_unlock(&thirdBuff_mutex);
		memset(output, '\0', strlen(output)); //just in case

			if(donecmd){
						//printf("plus BROKE\n");

				break; //proccess everything THEN quit, not before output has to recieve the DONE 
			}
}
	return NULL;
}



void* inputFunc(void* num){
	char copyString[10000];

	int i; //break BOOLEAN
	int produce_check = 0;
	while(1){


		i = 0;
		fgets(copyString, 10000, stdin); //take in string 10000 at a time

		if(copyString[0] == 'D' && copyString[1] == 'O'  && copyString[2] == 'N'  && copyString[3] == 'E' && copyString[4] == '\n'){
			i = 1; //break at the end
		}

		pthread_mutex_lock(&firstBuff_mutex);
		while(buff1Size != 0) //producer check for input func
		pthread_cond_wait(&busy, &firstBuff_mutex); //wait if theres something there for consumer to take

		strcpy(buffer1, copyString); //send what I just got to 
		buff1Size = strlen(buffer1); //set comparison
		pthread_cond_signal(&done); //signal
		pthread_mutex_unlock(&firstBuff_mutex); //unlock
		memset(copyString, '\0', strlen(copyString)); //just in case

		if(i == 1){
							//printf("input BROKE\n");

			break; //break condition (DONE)
		}
	}
	return NULL;
}


void *lineSeperator(void* num){

	char copyString[10000];
	char newLineFix[10000];
	int i = 0;
		int donecmd = 0;
	while(1){

		pthread_mutex_lock(&firstBuff_mutex);
		while(0 == buff1Size) //if there is nothing from the producer, just wait
		pthread_cond_wait(&done, &firstBuff_mutex); 

		strcpy(copyString, buffer1); //take input from Inputfunc
		
		memset(buffer1, '\0', strlen(buffer1)); //set it back to nothing 
		buff1Size = strlen(copyString); //set the check to say theres something in there
		pthread_cond_signal(&busy);
		pthread_mutex_unlock(&firstBuff_mutex);

		if(copyString[0] == 'D' && copyString[1] == 'O'  && copyString[2] == 'N'  && copyString[3] == 'E' && copyString[4] == '\n'){
			donecmd = 1; //check for done
		}
		if(copyString[0] == ' ' && copyString[1] == 'D' && copyString[2] == 'O'  && copyString[3] == 'N'  && copyString[4] == 'E' && copyString[5] == ' '){
			donecmd = 1; //check for a done command
		}
					i = 0;
			while(copyString[i] != '\0'){ //while we are in the string
				
				if(copyString[i] == '\n'){
					newLineFix[i] = ' '; //change all new lines to ' '
				}
				newLineFix[i] = copyString[i]; //if its not a newline, just copy the char
				i++;
			}
			newLineFix[strcspn(newLineFix, "\n")] = ' '; //https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input

			i = 0;


		pthread_mutex_lock(&secondBuff_mutex); //lock

		while(buff2Size != 0) //wait for nothing to be in buff 2
		pthread_cond_wait(&busy1, &secondBuff_mutex); //wait

		strcpy(buffer2, newLineFix); //send 

		buff2Size = strlen(copyString); //change
		pthread_cond_signal(&done1); //signal
		pthread_mutex_unlock(&secondBuff_mutex); //unlock
		memset(newLineFix, '\0', strlen(newLineFix)); //just in case
		memset(copyString, '\0', strlen(copyString)); //just in case


			if(donecmd){
								//printf("line BROKE\n");
								//printf("%s\n", newLineFix);

				break; //if the done check was triggered
			}



}
	return NULL;
}

int main(){


	int* var;
	int fail = 0;
	int* notvar; //for some reason unlike the example I couldnt get my code to compile
	//if I just passed NULL, so I had to make a useless var

	pthread_t uno, dos, tres, quatro; //sorry for such bad names, this main was A LOT more complicated at one point

	fail = pthread_create(&uno, NULL, inputFunc, (void*) var); //create thread call it to INPUTFUNC
		if(fail != 0){ 
			fprintf(stderr, "THREAD 1 DID NOT CREATE\n" );
		}
	fail = pthread_create(&dos, NULL, lineSeperator, (void*) notvar); //create thread call it to lineseperator
		if(fail != 0){
			fprintf(stderr, "THREAD 2 DID NOT CREATE\n" );
		}
			fail = pthread_create(&tres, NULL, plusSignFix, (void*) notvar); //create thread call it to plus sign destroyer
		if(fail != 0){
			fprintf(stderr, "THREAD 3 DID NOT CREATE\n" );
		}
			fail = pthread_create(&quatro, NULL, output, (void*) notvar); //create thread call it to the output func
		if(fail != 0){
			fprintf(stderr, "THREAD 4 DID NOT CREATE\n" );
		}
	pthread_join(uno, NULL); //wait for all of these to end
	pthread_join(dos, NULL); // they are all in while(1) loops and only end if a DONE cmd is sent.
	pthread_join(tres, NULL);
	pthread_join(quatro, NULL);

}