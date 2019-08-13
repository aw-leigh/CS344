#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>     /* process IDs */
#include <dirent.h>     /* finding directories */
#include <time.h>       /* time output */
#include <pthread.h>    /* mutex and threads*/

/* no magic numbers here! */
#define numberOfRooms 7
#define maxConnections 6

/* mutex for time function */
pthread_mutex_t timeMutex = PTHREAD_MUTEX_INITIALIZER;

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
void printLocation(struct room * rooms[], int roomNumber, bool timeBool);
int getLocation(struct room * rooms[], char * location);
void runGame(struct room * rooms[]);
void* exportTime();
void printTime();

/**********************/
/**** MAIN FUNCTION ***/
/**********************/

int main()
{
    int i;
    struct room * rooms[numberOfRooms];
    char newestFolder[30];
    memset(newestFolder, '\0', 30);

    /* lock mutex so that main get the thread */
    pthread_mutex_lock(&timeMutex);

    findNewestDirectory(newestFolder);
    makeArrayFromFiles(rooms, newestFolder);
    runGame(rooms);

    /* always free allocated memory */
    for(i = 0; i < numberOfRooms; i++){
        free(rooms[i]);
    }
    
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
void printLocation(struct room * rooms[], int roomNumber, bool timeBool){
    int i;

    if(timeBool == false){
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

/* runs the actual game, printing to console and asking for input */
void runGame(struct room * rooms[]){
    int i;
    int steps = 0;
    int location = 0;    
    bool goodInput;
    bool timeBool = false; /* used to skip re-printing the room after user asks for time */
    char *buffer;
    size_t bufferSize = 64;
    size_t input;
    char pathToVictory[1024];
    memset(pathToVictory, '\0', 1024);    

    buffer = (char *)calloc(bufferSize, sizeof(char));
    if(buffer == NULL){
        perror("Unable to allocate buffer\n");
        exit(1);
    }

    /* create a second thread, will not execute right away due to lock */
    pthread_t timeThread;
    pthread_create(&timeThread, NULL, exportTime, NULL);

    do{
        goodInput = false;
        printLocation(rooms, location, timeBool);
        timeBool = false;

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
            else if(strcmp(buffer, "time") == 0){
                memset(buffer, '\0', bufferSize); /* clear buffer */
                pthread_mutex_unlock(&timeMutex); /* unlock mutex so other thread can jump in */
                pthread_join(timeThread, NULL); /* block main until timeThread finishes */
                pthread_mutex_lock(&timeMutex); /* re-lock mutex for main */
                pthread_create(&timeThread, NULL, exportTime, NULL); /* re-create time thread */
                printTime();
                goodInput = true;
                timeBool = true;
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

    free(buffer);
}

/* time format: 1:03pm, Tuesday, September 13, 2016 */
void* exportTime(){
    /* this thread gets the lock */
	pthread_mutex_lock(&timeMutex);

    int i = 0;
    char * timeFilename = "currentTime.txt";
    char timeString[128];
    memset(timeString, '\0', 128);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    /* grab time string */
    strftime(timeString, 128, "%I:%M%p, %A, %B %d, %G", &tm);
    
    /* set AM/PM to lowercase */
    timeString[5] = tolower(timeString[5]);
    timeString[6] = tolower(timeString[6]);

    /* if there's a leading 0 on the hour, shift string to the left */
    if(timeString[0] == 48){ /* '48' is ASCII of 0 */
        while(timeString[i]){
            timeString[i] = timeString[i+1];
            i++;
        }
    }

    /* open the file for writing (erroring if error), print the time, and close the file */
    FILE *f = fopen(timeFilename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "%s\n", timeString);
    fclose(f);

	/* Unlock mutex and main to continue */
	pthread_mutex_unlock(&timeMutex);
}

void printTime(){

    char buffer[255];
    memset(buffer, '\0', 255);

    FILE *f = fopen("currentTime.txt", "r");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fgets(buffer,255,f);
    printf("\n %s", buffer);
}