/** 
  * File:    andesore.adventure.c 
  * 
  * Author1:  Soren Andersen (andesore@oregonstate.edu) 
  * Date:     Summer 2020
  * Partner:  I worked alone  
  * Course:   Computer Science 344 
  * 
  * Summary of File: 
  * 
  * Uses a folder and 7 files to play game 
  * 
  * COMMENT STYLE TAKEN FROM: https://www.cs.utah.edu/~germain/PPS/Topics/commenting.html
  *
  */ 
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

struct Room{

	int id;
	int connects[6];
	int totalconnects;
	char name[13];
	char roomType[15];
};




 /*THE CODE AND COMMENTS BELOW ARE FROM MODULE "Files & Directories II" 
 https://oregonstate.instructure.com/courses/1806251/pages/exploration-files-and-directories-ii?module_item_id=19760577
 FROM IN CLASS. Taken on 7/6/2020.
/*
* Go to the directory specified by path. In this direcory,
* look for all the directories whose name start with prefix,
* return the name of the directory that has the latest
* modification time.
*/
char* findLatestDirectory(char* path, char* prefix){
  struct stat dirStat;
  char directoryName[256];
  char* latestDirName;

  // Open the directory
  DIR* currDir = opendir(path);
  struct dirent *aDir;
  time_t lastModifTime;
  int i = 0;

  /* The data returned by readdir() may be overwritten by subsequent calls to  readdir()  for  the  same  directory stream. So we will copy the name of the directory to the variable directoryName
  */ 
  while((aDir = readdir(currDir)) != NULL){
    // Use strncmp to check if the directory name matches the prefix
    if(strncmp(prefix, aDir->d_name, strlen(prefix)) == 0){
      stat(aDir->d_name, &dirStat);
      // Check to see if this is the directory with the latest modification time
      if(i == 0 || difftime(dirStat.st_mtime, lastModifTime) > 0){
        lastModifTime = dirStat.st_mtime;
        memset(directoryName, '\0', sizeof(directoryName));
        strcpy(directoryName, aDir->d_name);
      }
      i++;
    }
  }
  latestDirName = malloc(sizeof(char) * (strlen(directoryName) + 1));
  strcpy(latestDirName, directoryName);

  closedir(currDir);
  return latestDirName;
}
/** 
  * 
  * void getRoomNames(char* folder, struct Room* rooms)
  * 
  * Summary of the getRoomNames function: 
  * 
  *    Gets room names from file headers
  * 
  * Parameters   : directory of files name and current room struct 
  * 
  * Return Value : Nothing  
  * 
  * Description: 
  * 
  *    This function just takes the directory its currently in file names
  * 	This is just a modified verison of the code above to get the file name
  *		Aka this is almost from https://oregonstate.instructure.com/courses/1806251/pages/exploration-files-and-directories-ii?module_item_id=19760577
  */ 
 
void getRoomNames(char* folder, struct Room* rooms){


	DIR *dir;
	struct dirent *path;
	int file = 0;

	if((dir = opendir(folder)) != NULL){
		while((path = readdir(dir)) != NULL){
			if(strlen(path->d_name) > 2){
				strcpy(rooms[file].name, path->d_name);
				rooms[file].id = file;
				file++;
			}
		}
	}


}
 /** 
  * 
  * int findID(char* name, struct Room* rooms) 
  * 
  * Summary of the findID function: 
  * 
  * Find's an ID based upon the name of the room 
  * 
  * Parameters   : Name of a room and the room struct 
  * 
  * Return Value : Returns the ID number of the room with the given name 
  * 
  * Description: 
  * 
  * Simply compares names in the room struct until it reaches the desired name
  * then returns the id number of that room
  * 
  */ 

int findID(char* name, struct Room* rooms){
	int i = 0;
	char* temp_room = malloc(sizeof(char) * 12);
	char* third_temp = malloc(sizeof(char) * 12);

	for(i=0; i < 7; i++){
	strcpy(temp_room, rooms[i].name);
		strcpy(third_temp, strtok(temp_room, "_")); //Usage came from modules and https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
		if(strcmp(third_temp, name) == 0){ //strcmp == 0 thanks to http://www.cplusplus.com/reference/cstring/strcmp/
			return i;
		}
	}
	return -1;
}

 /** 
  * 
  * void getFullStats(struct Room* rooms, int* special_id) 
  * 
  * Summary of the getFullStats function: 
  * 
  * Rebuilds the room struct from the information in the files
  * 
  * Parameters   : room struct and the ID of the END and START rooms to tag them correctly
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  * Once the folder is found, goes file by file extracting information from each line
  * Just follows algorithim set up by file creation.
  * 
  */ 
void getFullStats(struct Room* rooms, int* special_id){

	int i = 0;
	int j = 0;
	FILE* roomName;
	char rename[75];
	char sprint_thru;
	int num_connects = 0;
	char* linebyline;
  	for(i = 0; i < 7; i++){

	sprintf(rename, "./%s", rooms[i].name);
	roomName = fopen(rename, "r");

	if(roomName == NULL){
		return; //If the file isn't found
	}

	fseek(roomName, 0, 0);
	sprint_thru = getc(roomName);
	num_connects = 0;


	while(sprint_thru != EOF){ // EOF = End of file, taken from in class material
		if(sprint_thru == '\n'){
			num_connects++;
		}
		sprint_thru = getc(roomName);

	}

		fseek(roomName, 0, 0); //Reset where the file pointer is


		num_connects = num_connects - 2; //Cause first and second line arent connects

	rooms[i].totalconnects = num_connects;

	fgets(rename, 75, roomName);
	linebyline = strtok(rename, ":");  //https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
	linebyline = strtok(NULL, "\n");   //Strtok use learned from ^^

	//All strtok use is to strip the stuff before the value 

	for (j = 0; j < num_connects; j++)
	{
		fgets(rename, 75, roomName); //this is for each connection, since there is a dynamic amount of them
	linebyline = strtok(rename, ":"); 
	linebyline = strtok(NULL, "\n");
	linebyline++; //Ok so strtok doesn't take all the blank space out, so everything comes from the file with one space in front
					// So I got around that by just upping the index by 1 to ignore that space for the string (dont blame me blame c strings)
	rooms[i].connects[j] = findID(linebyline, rooms); //Find ID comes in useful

	if(j == (num_connects - 1)){ //Special case for last connection, instructions state the last one needs a period after it.
		fgets(rename, 75, roomName);
		linebyline = strtok(rename, ":");
		linebyline = strtok(NULL, "\n");
		linebyline++;
		strcpy(rooms[i].roomType, linebyline);
		if(strcmp(rooms[i].roomType, "START_ROOM") == 0){ //also set the START and END room since this is a convienent time
			special_id[0] = rooms[i].id ;
		}
		else if(strcmp(rooms[i].roomType, "END_ROOM") == 0){
			special_id[1] = rooms[i].id ;
		}
		break;
	}
	}


	fclose(roomName); //gotta close the file after
}

}

 /** 
  * 
  * void fullStatsPrint(struct Room* rooms) 
  * 
  * Summary of the fullStatsPrint function: 
  * 
  * This is just a helper function for me to make sure all the infos accurate
  * 
  * Parameters   : Room struct 
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  * Like I said this isn't in use but if you want to see all the accurate info you can write it in
  * Was useful for debugging
  * 
  */ 
void fullStatsPrint(struct Room* rooms){
	int i = 0;
	for(i =0; i < 7; i++){
		printf("Room %d Name: %s\n", rooms[i].id, rooms[i].name);
		printf("Room %d Has: %d total connections\n", rooms[i].id, rooms[i].totalconnects );
			int j = 0;
			for (j = 0; j < rooms[i].totalconnects; j++)
				{
					printf("Connection %d is: %d\n", j, rooms[i].connects[j]);
				}	

		printf("Its type is: %s\n\n", rooms[i].roomType);
	}
}

 /** 
  * 
  * void game(struct Room* rooms, int* special_id) 
  * 
  * Summary of the game function: 
  * 
  * This is the function that deals with all the game related things

  * Parameters   : Room struct and special_id's just in case
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  * Ok so largely this function just prints thing out. Its all in a do while loop
  * based around finding the END room (winning the game). But it prints out the info according to the style guide
  * then just waits for a user input.
  * 
  */ 
void game(struct Room* rooms, int* special_id){

	int currentRoom = special_id[0];
	int i = 0;
	char userInput[10]; //Ok so I arbtrarily just choose 10 steps cause if anyone plays this for longer thats on them.
						// But the instructions never said anything about max length or a losing condition
	int step_count = 0;
	int steps_taken[10];
	do{
		printf("CURRENT LOCATION: %s\n", rooms[currentRoom].name);
		printf("POSSIBLE CONNECTIONS: ");
			for (i = 0; i < rooms[currentRoom].totalconnects; i++) //Based around current room connection number
			{
				if(i == rooms[currentRoom].totalconnects - 1){
					printf("%s. ", rooms[rooms[currentRoom].connects[i]].name); //Prints the current room connection's names
					printf("\n");
					break;
				}
				printf("%s, ", rooms[rooms[currentRoom].connects[i]].name);

			}
		printf("WHERE TO? >"); 
		scanf("%s", userInput); //Used scanf not getline because we were taught that way in data structs
		printf("\n");

		if(strlen(userInput) > 2){ // Really just length more than 1, but that didn't work 
			for (i = 0; i < rooms[currentRoom].totalconnects; i++) //loop until you find the name then move it to that ID
			{
				if(strcmp(rooms[rooms[currentRoom].connects[i]].name, userInput) == 0){
					currentRoom = rooms[currentRoom].connects[i]; 
					steps_taken[step_count] = currentRoom;
					step_count++; //Simple stat tracking
					break;
				}
				else if(strcmp(rooms[rooms[currentRoom].connects[i]].name, userInput) != 0 && i != rooms[currentRoom].totalconnects - 1){
					continue;
				}
				else if(i == rooms[currentRoom].totalconnects - 1){ //if the ID isn't found by the last room ID, call it quits.
					printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
				}
			}

}


	}while(strcmp(rooms[currentRoom].roomType, "END_ROOM") != 0); //WIN CONDITION


	printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS: \n", step_count);
	for (i = 0; i < step_count; i++)
	{
		 printf("%s\n", rooms[steps_taken[i]].name); //List path taken
	}
}

 /** 
  * 
  * void nameStrip(struct Room* rooms)
  * 
  * Summary of the nameStrip function: 
  * 
  * Simple program to extract just the name of each room not the full file title
  * Parameters   : Current Room names
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  * Uses strtok to strip name from file names.
  * 
  */ 
void nameStrip(struct Room* rooms){
	int i = 0;

	for (i = 0; i < 7; i++)
	{
	char* temp = malloc(strlen(rooms[i].name) + 1);
	temp = strtok(rooms[i].name, "_");
	}
}
 /** 
  * 
  * int main() 
  * 
  * Summary of the main function: 
  * 
  * Mainly just call's functions and creates constants 
  * 
  * Parameters   : None
  * 
  * Return Value : 0 
  * 
  * Description: 
  * 
  * First creates the file path nessecary, then calls the directory based upon class code
  * The rooms are created next, then the names of each room are found based upon the file names
  * After that all the other data is stripped from each file
  * Finally the game can be played as we have all the info from each file.
  * Returns 0 like the instructions say
  * 
  */ 
int main(){
	char* prefix = "andesore.rooms.";
	char* path = "./";

	int i = 0;
	int special_id[2] = {-1,-1};

	char* answer = findLatestDirectory(path, prefix );

	struct Room rooms[7];

	
  	getRoomNames(answer, rooms);

  	chdir(answer);
  		getFullStats(rooms, special_id);
  		nameStrip(rooms);

  		game(rooms, special_id);

  		return 0;
  	//	fullStatsPrint(rooms);

}




