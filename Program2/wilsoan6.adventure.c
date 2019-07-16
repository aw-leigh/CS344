#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> 

/* no magic numbers here! */
#define numberOfRooms 7
#define maxConnections 6

/* reference: https://stackoverflow.com/a/1921557 */
typedef int bool;
enum { false, true };

struct room
{
    char roomName[16];
    char roomType[16];
    int numConnections;
    char connectionNames[maxConnections][16];
};

/**********************/
/** FUNCTION HEADERS **/
/**********************/

void findNewestDirectory(char newestFolder[]);
void makeArrayFromFiles(struct room * rooms[], char newestFolder[]);
void printLocation(struct room * rooms[], int roomNumber);
int getLocation(struct room * rooms[], char * location);

/**********************/
/**** MAIN FUNCTION ***/
/**********************/

int main()
{
    int i, j;
    int steps = 0;
    int location = 0;
    struct room * rooms[numberOfRooms];
    char newestFolder[30];
    memset(newestFolder, '\0', 30);
    char pathToVictory[1024];
    memset(pathToVictory, '\0', 1024);

    findNewestDirectory(newestFolder);
    makeArrayFromFiles(rooms, newestFolder);

    /*************************/
    /****** GAME LOGIC *******/
    /*************************/

    bool goodInput;
    char *buffer;
    size_t bufferSize = 64;
    size_t input;

    buffer = (char *)calloc(bufferSize, sizeof(char));
    if(buffer == NULL){
        perror("Unable to allocate buffer\n");
        exit(1);
    }
    
    do{
        goodInput = false;
        printLocation(rooms, location);

        input = getline(&buffer, &bufferSize, stdin);   
        buffer[strcspn (buffer, "\n")] = '\0'; /* strip the trailing newline */
        
        for(i = 0; i < rooms[location]->numConnections; i++){
            
            /* compare input to connections */
            if(strcmp(buffer, rooms[location]->connectionNames[i]) == 0){
                location = getLocation(rooms, buffer);
                goodInput = true;
                steps++;
                strcat(pathToVictory,buffer);
                strcat(pathToVictory,"\n");
            }
        }
        if(goodInput == false){
            printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
        }
        printf("\n");

    }while(strcmp(rooms[location]->roomType, "END_ROOM") != 0);

    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
    printf("%s",pathToVictory);

    /* always free allocated memory */
    for(i = 0; i < numberOfRooms; i++){
        free(rooms[i]);
    }
    free(buffer);

    return 0;
}

/* takes a char array to store name of most recently modified directory in the current directory */
void findNewestDirectory(char newestFolder[]){
    
    int folderModifyTime = 0;

    DIR *d;
    struct dirent *dir;
    struct stat fileStats;

    /* check the current directory */
    d = opendir(".");
    if(d){
        
        /* check each file */
        while ((dir = readdir(d)) != NULL) {

            /* if the file is a directory, and is not "." or ".." */
            if(dir->d_type == DT_DIR && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0){

                /* put stats of directory into fileStats */
                stat(dir->d_name, &fileStats);

                /* st_mtime is an integer, larger values = modified more recently */
                /* track the largest one, and save the corresponding directory to newestFolder */
                if(fileStats.st_mtime > folderModifyTime){
                    folderModifyTime = fileStats.st_mtime;
                    strcpy(newestFolder, dir->d_name);
                }
            }
        }
        closedir(d);
    }
    /*printf("Newest directory: %s\n", newestFolder);*/
}

/* takes an array to store room info and a string with the path to the files to open */
void makeArrayFromFiles(struct room * rooms[], char newestFolder[]){
    int i;
    
    bool roomBool = false; /* to track whether to save room to type or name */
    char buffer[255];
    memset(buffer, '\0', 255);
    char fileName[30];
    memset(fileName, '\0', 30);

    char tempString1[30];
    memset(tempString1, '\0', 30);    
    char tempString2[30];
    memset(tempString2, '\0', 30);       
    
    /* reconstruct the room array, one room at a time */
    for(i = 0; i < numberOfRooms; i++){
        
        /* allocate a room and initialize variables */
        struct room * newRoom = malloc(sizeof(struct room));
        newRoom->numConnections = 0;
        memset(newRoom->roomName, '\0', 16);
        memset(newRoom->roomType, '\0', 16);

        /* get the name of each file, including the directory name */       
        sprintf(fileName, "./%s/room%d", newestFolder, i);
        FILE *f = fopen(fileName, "r");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }

        /* read line-by-line into the buffer */
        while(fgets(buffer, 255, f)) {
            sscanf(buffer, "%s %*s %s", tempString1, tempString2);
            /*printf("  %s", tempString1);
            printf(" %s\n", tempString2);*/
            if(strcmp(tempString1, "ROOM") == 0 && roomBool == false){
                strcpy(newRoom->roomName, tempString2);
                roomBool = true;
            }
            else if(strcmp(tempString1, "ROOM") == 0 && roomBool == true){
                strcpy(newRoom->roomType, tempString2);
                roomBool = false;
            }
            else if(strcmp(tempString1, "CONNECTION") == 0){
                strcpy(newRoom->connectionNames[newRoom->numConnections], tempString2);
                newRoom->numConnections++;
            }             
        }
        rooms[i] = newRoom;
        fclose(f);
    }
    
    /*Print room1 stats and connections to stdout, for testing*/
    /*
    for(i = 0; i < numberOfRooms; i++){
        printf("Room %d\n  Connections: %d\n  Room Name: %s\n  Room Type: %s\n", i, rooms[i]->numConnections, rooms[i]->roomName, rooms[i]->roomType);
    }
    printf("Start Room Connections\n");
    for(i = 0; i < rooms[0]->numConnections; i++){
        printf("  Connection %d: %s\n", i+1, rooms[0]->connectionNames[i]);
    }
    */
}

/* prints user interface with correct punctuation */
void printLocation(struct room * rooms[], int roomNumber){
    int i;

    printf("CURRENT LOCATION: %s\n", rooms[roomNumber]->roomName);
    printf("POSSIBLE CONNECTIONS: ");
    for(i = 0; i < rooms[roomNumber]->numConnections; i++){
        printf("%s", rooms[roomNumber]->connectionNames[i]);
        if(i == rooms[roomNumber]->numConnections - 1){
            printf(".\n");
        }
        else{
            printf(", ");
        }
    }
    /* printf("Debug: Type = %s\n", rooms[roomNumber]->roomType); */
    printf("WHERE TO? >");

}

/* returns index of room named "location" in array "rooms" */
int getLocation(struct room * rooms[], char * location){
    int i;

    for(i = 0; i < numberOfRooms; i++){
        if(strcmp(location, rooms[i]->roomName) == 0){
            return i;
        }
    }
    return -1;
}