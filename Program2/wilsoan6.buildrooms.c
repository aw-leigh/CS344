#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/* no magic numbers here! */
#define numberOfRooms 7
#define maxConnections 6

/* reference: https://stackoverflow.com/a/1921557 */
typedef int bool;
enum { false, true };

struct room
{
    char *roomName;
    char *roomType;
    int numConnections;
    struct room *connections[maxConnections];
};

/**********************/
/** FUNCTION HEADERS **/
/**********************/

bool IsGraphFull(struct room * rooms[], int numRooms);
void AddRandomConnection(struct room * rooms[], int numRooms);
struct room *GetRandomRoom(struct room * rooms[], int numRooms);
bool CanAddConnectionFrom(struct room * x);
bool ConnectionAlreadyExists(struct room * x, struct room * y);
void ConnectRoom(struct room * x, struct room * y);
bool IsSameRoom(struct room * x, struct room * y);
void ShuffleRoomNames(char ** possibleRoomNames, int numPossibleRoomNames);

/**********************/
/**** MAIN FUNCTION ***/
/**********************/

int main()
{
    int i, j;
    struct room * rooms[numberOfRooms];
    int seed = time(NULL); /* seed random number gen used in room name shuffle */
    srand(seed);

    /**********************/
    /**** CREATE ROOMS ****/
    /**********************/

    char *possibleRoomNames[10] = {"Vivarium", "Lake", "Mill", "Academy", "Elevator", "Village", "Hospital", "Carnival", "Grotto", "Island"};
    i = 10;

    ShuffleRoomNames(possibleRoomNames, i);

    /* create rooms */
    struct room startRoom = {possibleRoomNames[0], "START_ROOM", 0};
    struct room endRoom = {possibleRoomNames[1], "END_ROOM", 0};
    struct room midRoom1 = {possibleRoomNames[2], "MID_ROOM", 0};
    struct room midRoom2 = {possibleRoomNames[3], "MID_ROOM", 0};
    struct room midRoom3 = {possibleRoomNames[4], "MID_ROOM", 0};
    struct room midRoom4 = {possibleRoomNames[5], "MID_ROOM", 0};
    struct room midRoom5 = {possibleRoomNames[6], "MID_ROOM", 0};

    /* add rooms to array */
    rooms[0] = &startRoom;
    rooms[1] = &endRoom;
    rooms[2] = &midRoom1;
    rooms[3] = &midRoom2;
    rooms[4] = &midRoom3;
    rooms[5] = &midRoom4;
    rooms[6] = &midRoom5;

    /* Create all connections in graph */
    while (IsGraphFull(rooms, numberOfRooms) == false){
        AddRandomConnection(rooms, numberOfRooms);
    }

    /* Print room1 stats and connections to stdout, for testing
    for(i = 0; i < numberOfRooms; i++){
        printf("Room %d\nnumConnections: %d\nroomName: %s\nroomType: %s\n", i, rooms[i]->numConnections, rooms[i]->roomName, rooms[i]->roomType);
    }
    for(i = 0; i < rooms[0]->numConnections; i++){
        printf("Start Room Connections\n");
        printf("Connection %d: %s\n", i+1, rooms[0]->connections[i]->roomName);
    }
    */

    /**********************/
    /**** CREATE FILES ****/
    /**********************/

    /* create folder name string, grab pid, append to folder name string, and make folder */
    char folderName[30];
    memset(folderName, '\0', 30);
    sprintf(folderName, "wilsoan6.rooms.%d", getpid());
    mkdir(folderName, 0755);

    /* create a file for each room */
    for(i = 0; i < numberOfRooms; i++){
        
        /* combine the path ("./"), folder name (%s/), and file name ("room%d") into a single string */
        char fileName[30];
        memset(fileName, '\0', 30);
        sprintf(fileName, "./%s/room%d", folderName, i);

        /* open the file, erroring if error */
        FILE *f = fopen(fileName, "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }

        /* print the name, connections, and type */
        fprintf(f, "ROOM NAME: %s\n", rooms[i]->roomName);
        for(j = 0; j < rooms[i]->numConnections; j++){
            fprintf(f, "CONNECTION %d: %s\n", j+1, rooms[i]->connections[j]->roomName);
        }
        fprintf(f, "ROOM TYPE: %s\n", rooms[i]->roomType);

        /* don't forget to close the file! */
        fclose(f);
    }

}

/**********************/
/** HELPER FUNCTIONS **/
/**********************/

void ShuffleRoomNames(char ** possibleRoomNames, int numPossibleRoomNames)
{
    int i, j;
    char * temp;

    for(i = numPossibleRoomNames - 1; i > 0; i--){
        j = rand() % (i+1);
        temp = possibleRoomNames[i];
        possibleRoomNames[i] = possibleRoomNames[j];
        possibleRoomNames[j] = temp;
    }
}

/* Returns true if all rooms have at least 3 connections, false otherwise */
bool IsGraphFull(struct room * rooms[], int numRooms)
{
    int i;

    for(i = 0; i < numRooms; i++){
        if(rooms[i]->numConnections < 3){
            return false;
        }
    }

    return true;
}

/* Adds a random, valid outbound connection from a Room to another Room */
void AddRandomConnection(struct room * rooms[], int numRooms)
{
    /* Pointers to hold rooms to add connections to */
    struct room *A;
    struct room *B;

    while (true)
    {
        A = GetRandomRoom(rooms, numRooms);

        if (CanAddConnectionFrom(A) == true)
            break; 
    }

    do
    {
        B = GetRandomRoom(rooms, numRooms);
    } while (CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

    ConnectRoom(A, B);
}

/* Returns a random Room, does NOT validate if connection can be added */
struct room * GetRandomRoom(struct room * rooms[], int numRooms)
{
    int i = rand() % (numRooms);
    return rooms[i];
}

/* Returns true if a connection can be added from roomIn (< 6 outbound connections), false otherwise */
bool CanAddConnectionFrom(struct room *roomIn)
{
    if (roomIn->numConnections < 6)
    {
        return true;
    }
    return false;
}
/* Returns true if a connection from Room x to Room y already exists, false otherwise */
bool ConnectionAlreadyExists(struct room * x, struct room * y)
{
    int i;
    for(i = 0; i < x->numConnections; i++){
        if(strcmp(x->connections[i]->roomName, y->roomName) == 0){
            return true;
        }
    }
    return false;
}

/* Connects Rooms x and y together, does not check if this connection is valid */
void ConnectRoom(struct room *x, struct room *y)
{
    x->connections[x->numConnections] = y;
    x->numConnections++;

    y->connections[y->numConnections] = x;
    y->numConnections++;
}

/* Returns true if Rooms x and y are the same Room, false otherwise */
bool IsSameRoom(struct room *x, struct room *y)
{
    if(strcmp(x->roomName, y->roomName) == 0){
        return true;
    }
    return false;
}