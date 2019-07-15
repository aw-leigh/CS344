#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


//reference: https://stackoverflow.com/a/1921557
typedef int bool;
enum { false, true };

struct room
{
    char *roomName;
    char *roomType;
    int numConnections;
    struct room *connections[6];
};

bool IsGraphFull();
void AddRandomConnection();
struct room GetRandomRoom();
bool CanAddConnectionFrom(struct room x);
bool ConnectionAlreadyExists(struct room x, struct room y);
void ConnectRoom(struct room x, struct room y);
bool IsSameRoom(struct room x, struct room y);
void shuffleRoomNames(char ** roomNameArray, int numRooms);

int main()
{
    int i, j;

    /* seed random number gen used in room name shuffle */
    int seed = time(NULL);
    srand(seed);

    /* create folder name string, grab pid, append to folder name string, and make folder */
    char folderName[30];
    memset(folderName, '\0', 30);
    sprintf(folderName, "wilsoan6.rooms.%d", getpid());
    mkdir(folderName, 0755);

    char *possibleRoomNames[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    i = 10;

    shuffleRoomNames(possibleRoomNames, i);

    /* create rooms */
    struct room startRoom = {possibleRoomNames[0], "START_ROOM", 0};
    struct room endRoom = {possibleRoomNames[1], "END_ROOM", 0};
    struct room midRoom1 = {possibleRoomNames[2], "MID_ROOM", 0};
    struct room midRoom2 = {possibleRoomNames[3], "MID_ROOM", 0};
    struct room midRoom3 = {possibleRoomNames[4], "MID_ROOM", 0};
    struct room midRoom4 = {possibleRoomNames[5], "MID_ROOM", 0};
    struct room midRoom5 = {possibleRoomNames[6], "MID_ROOM", 0};

    /* Create all connections in graph 
    while (IsGraphFull() == false)
    {
        AddRandomConnection();
    }*/

    printf("Start Room\nnumConnections: %d\nroomName: %s\nroomType: %s\n", startRoom.numConnections, startRoom.roomName, startRoom.roomType);
}

void shuffleRoomNames(char ** roomNameArray, int numRooms)
{
    int i, j;
    char * temp;

    for(i = numRooms - 1; i > 0; i--){
        j = rand() % (i+1);
        temp = roomNameArray[i];
        roomNameArray[i] = roomNameArray[j];
        roomNameArray[j] = temp;
    }
}

// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
bool IsGraphFull()
{
    
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection()
{
    struct room A; // Maybe a struct, maybe global arrays of ints
    struct room B;

    while (true)
    {
        A = GetRandomRoom();

        if (CanAddConnectionFrom(A) == true)
            break;
    }

    do
    {
        B = GetRandomRoom();
    } while (CanAddConnectionFrom(B) == false || IsSameRoom(A, B) == true || ConnectionAlreadyExists(A, B) == true);

    ConnectRoom(A, B); // TODO: Add this connection to the real variables,
    ConnectRoom(B, A); //  because this A and B will be destroyed when this function terminates
}

// Returns a random Room, does NOT validate if connection can be added
struct room GetRandomRoom()
{
    
}

// Returns true if a connection can be added from roomIn (< 6 outbound connections), false otherwise
bool CanAddConnectionFrom(struct room roomIn)
{
    if (roomIn.numConnections < 6)
    {
        return true;
    }
    return false;
}
// Returns true if a connection from Room x to Room y already exists, false otherwise
bool ConnectionAlreadyExists(struct room x, struct room y)
{
    
}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct room x, struct room y)
{
    
}

// Returns true if Rooms x and y are the same Room, false otherwise
bool IsSameRoom(struct room x, struct room y)
{
    
}