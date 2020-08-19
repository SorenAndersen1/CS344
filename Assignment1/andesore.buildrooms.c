/** 
  * File:    andesore.buildrooms.c 
  * 
  * Author1:  Soren Andersen (andesore@oregonstate.edu) 
  * Date:     Summer 2020
  * Partner:  I worked alone  
  * Course:   Computer Science 344 
  * 
  * Summary of File: 
  * 
  * Builds rooms meant for andesore.adventure.c, should have no print to console. Makes a folder and 7 files.
  * 
  * COMMENT STYLE TAKEN FROM: https://www.cs.utah.edu/~germain/PPS/Topics/commenting.html
  */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>




//I didn't really get creative with this struct, its only the info that the instructions say we need
//Plus it has an id so I can easily identify without checking the name. The connections of each room is stored
// by connects array in which the id of each room the room is connected to is an index (doesn't have to fill all 6 spots)

struct Room{

	int id;
	int connects[6];
	int totalconnects;
	char name[10];
	char roomType[15];
};

 /** 
  * 
  * void connectRooms(struct Room* rooms, int idRoom, int roomID) 
  * 
  * Summary of the connectRooms function: 
  * 
  * Connects rooms if able to be connected
  * 
  * Parameters   : Room struct, and 2 different room IDs desired to be connected
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  * Ok so this function is used with random values, so first it is checked if the two rooms can even be connected
  * if they both have under 7 connections then the two rooms are connected to each other and the connection # goes up for each 
  * 
  */ 
void connectRooms(struct Room* rooms, int idRoom, int roomID){
 

	int roomCheck = 0; //boolean val
						

	if(rooms[idRoom].totalconnects < 7 && rooms[roomID].totalconnects < 7){ //Max of 6 connections
		int i = 0;

			for(i=0; i < rooms[idRoom].totalconnects; i++){// Second check
				if(rooms[idRoom].connects[i] == roomID){//Seeing if one is connected to the other already
					roomCheck = 0; //if so dip out

					break;
				}
				else{

					roomCheck = 1; //if Not all good to connect go ahead
				}

			}

	}
				if (rooms[idRoom].totalconnects == 0)
			{
				roomCheck = 1; //Had to add a base case I got some weird behavior
			}
	if(roomCheck == 1){
		rooms[idRoom].connects[rooms[idRoom].totalconnects] = roomID;
		rooms[roomID].connects[rooms[roomID].totalconnects] = idRoom; //simple trading of information
		rooms[idRoom].totalconnects++;
		rooms[roomID].totalconnects++;
	}

}
 /** 
  * 
  * int leastConnections(struct Room* rooms, int firstID, int secondID)
  * 
  * Summary of the leastConnections function: 
  * 
  * Used to check all rooms have more than 2 connections and less than 7
  * 
  * Parameters   : Room struct and 2 ID's to check even tho it only checks one. Genius design.
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  *  Finds if the firstID has enough connections to fit the instructions
  * 
  */ 
int leastConnections(struct Room* rooms, int firstID, int secondID){
	int i, return_val = 0;
	  int c_var = rooms[firstID].totalconnects;
	  int repeat = rooms[secondID].totalconnects;
		if((c_var >= 3 && c_var < 7) ){

			return 0; //true
		}
		else{

			return 1; //false
		}
	}



void connects(struct Room* rooms);
 /** 
  * 
  * void room_construction(struct Room* rooms, char** room_names) 
  * 
  * Summary of the room_construction function: 
  * 
  * Constructs room based on random variables from the library
  * 
  * Parameters   : Room struct
  * 
  * Return Value : Nothing  
  * 
  * Description: 
  * 
  * First set all unused connects to -1, and set constants like ID's and totalconnects
  * Then use random values to set the rest using rand() func
  * 
  */ 
void room_construction(struct Room* rooms, char** room_names){

	int i, j, temp, randvar = 0;



	for(i=0; i < 7; i++){

		rooms[i].id = i;
		rooms[i].totalconnects = 0;

			for(j=0; j < 6; j++){
				rooms[i].connects[j] = -1;
			}

		do{
			temp = rand() % 10;
		}while(room_names[temp] == NULL); 

		strcpy(rooms[i].name, room_names[temp]);   
		room_names[temp] = NULL;//once a name is used set it to null
	}
		strcpy(rooms[0].roomType, "START_ROOM"); //Still random as names are random 
		strcpy(rooms[1].roomType, "END_ROOM"); //guess this copies each time which isnt perfect


			for(i=0; i < 7; i++){
		rooms[i].totalconnects = 0; //had some issues with this number so I assign it twice
			}


	 for(i=2; i < 7; i++){
		strcpy(rooms[i].roomType, "MID_ROOM"); //set the rest to mid rooms
	}	


	connects(rooms); //start the connecting process


}
 /** 
  * 
  * void connects(struct Room* rooms)
  * 
  * Summary of the connects function: 
  * 
  * Connects rooms
  * 
  * Parameters   : Room Struct
  * 
  * Return Value : Nothing 
  * 
  * Description: 
  * 
  *Ok so this is a function that loops 500 times, trying to connect random variables each time
  * Why 500? just the number I desired on as getting 6 connections for a room is rare.
  * 
  * 
  */ 
void connects(struct Room* rooms){
	int temp;
	int randvar;
	int i = 0;
	int j = 0;
	   	temp = (rand() % 5) + 3;

  	 for(i=0; i < 7; i++){ //each room 
  		for(j = 0; j < 500; j++){ //500 times each
 		
	// do{
	 	do{
	 	randvar = (rand() % 6) + 1; //get a random room

	 	}while(randvar == i); //Make sure its not connecting itself to itself


	if(leastConnections(rooms, i, randvar) == 1){ //make sure there arent too little connections
	 connectRooms(rooms, i, randvar); //actual connect the rooms/ check if the connection is valid
	}

	}
		   	temp = (rand() % 5) + 3; //start again with a number ID 

}




	 //}while(temp > rooms[i].totalconnects);

	
	
//  //so I dont connect a room to itself
// 		}
 }

 /** 
  * 
  * void printfunction(struct Room* rooms)
  * 
  * Summary of the printfunction function: 
  *  Prints all the data to individual files
  * 
  * 
  * Parameters   : Full room struct 
  * 
  * Return Value : Nothing  
  * 
  * Description: 
  * 
  * Based upon the instructions writes information to the files 
  * 
  */ 
void printfunction(struct Room* rooms){

	int j = 0;
	int i = 0;
	char string_write[50];

	for(i = 0; i < 7; i++){
		sprintf(string_write, "%s_room", rooms[i].name);
		FILE *write_file = fopen(string_write, "w"); //got this from modules

		fprintf(write_file, "ROOM NAME: %s\n", rooms[i].name);

		for(j=0; j < rooms[i].totalconnects; j++){
			fprintf(write_file, "CONNECTION %d: %s\n", j + 1, rooms[rooms[i].connects[j]].name ); //Simple connection listing
		}
		fprintf(write_file, "ROOM TYPE: %s\n", rooms[i].roomType);
	}

}
 /** 
  * 
  * int main()
  * 
  * Summary of the main function: 
  * Makes room struct and other constant then calls all functions
  * 
  * Parameters   : 
  * 
  * Return Value : 0
  * 
  * Description: 
  * 
  * room names are static, makes folder and folder name, then room struct then just calls functions
  * 
  */ 
int main()
{
	srand(time(NULL)); //set up random key

//static filenames
	char* room_names[10] = {"Parlor", "Ballroom", "Theatre", "Kitchen", "Bedroom", "Foyer", "Gym", "Lounge", "Dungeon", "Office"};

	//First thing to do, make folder so I can make files
	char folder_name[50]; 
	int file_num = rand() % 100000;

	sprintf(folder_name, "andesore.rooms.%d", file_num); //got sprintf use from cplusplus.com/reference/cstdio/sprintf/
		mkdir(folder_name, 0700);
			chdir(folder_name);


	struct Room rooms[7];
				room_construction(rooms, room_names); //construct rooms
				printfunction(rooms); //print newly constructed rooms to files
							

	return 0;


								
}