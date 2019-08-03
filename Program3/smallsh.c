#define _GNU_SOURCE //because I was getting an implicit declaration error with g++, reference: https://stackoverflow.com/a/8494003
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h> //for bools
#include <pwd.h>     //for finding HOME directory

// global variable
pid_t foregroundPID = -1;
bool foregroundOnlyMode = false;


void printExitStatus(int * status, bool * signalFlag){
    char *exitStatusMessage;

    if (*signalFlag == false)
    {
        exitStatusMessage = "exit value ";
    }
    else
    {
        exitStatusMessage = "terminated by signal ";
    }
    printf("%s%d", exitStatusMessage, *status);
    fflush(stdout);
}

void findZombies(int * status, bool * signalFlag){

    pid_t pid = NULL;
    int childExitMethod = -1337;

    //while there are any finished processes hanging around
    while ((pid = waitpid(-1, &childExitMethod, WNOHANG)) > 0){
        
        //return exit status value
        if (WIFEXITED(childExitMethod) != 0)
        {
            *signalFlag = false;
            *status = WEXITSTATUS(childExitMethod);
        }
        //return terminating signal value
        else if (WIFSIGNALED(childExitMethod) != 0)
        {
            *signalFlag = true;
            *status = WTERMSIG(childExitMethod);
        }
        
        printf("Background pid %d is done: ", pid);
        fflush(stdout);
        printExitStatus(status, signalFlag);   
    }
}

void findRedirections(char *userInputArray[], int numArguments, int *inPipeLocation, int *outPipeLocation)
{
    for (int i = 0; i < numArguments; i++)
    {
        if (strcmp(userInputArray[i], "<") == 0)
        {
            *inPipeLocation = i;
        }
        else if (strcmp(userInputArray[i], ">") == 0)
        {
            *outPipeLocation = i;
        }
    }
}

//returns -8888 if file could not be opened, otherwise returns file descriptor
int openFileForReading(char *userInputArray[], int *numArguments, int *inFileDescriptor, int inPipeLocation)
{

    //grab file path from the token following < and open file
    *inFileDescriptor = open(userInputArray[inPipeLocation + 1], O_RDONLY);

    //error and exit if file can't open
    if (*inFileDescriptor < 0)
    {
        perror("Unable to open input file\n");
        return -8888;
    }
    //otherwise, remove < symbol and filepath from array,
    //shifting everything over 2 places
    for (int i = inPipeLocation; i < *numArguments - 2; i++)
    {
        userInputArray[i] = userInputArray[i + 2];
    }
    *numArguments -= 2;

    return 0;
}

//returns -8888 if file could not be opened, otherwise returns file descriptor
int openFileForWriting(char *userInputArray[], int *numArguments, int *outFileDescriptor, int outPipeLocation)
{
    //grab file path from the token following > and open file
    *outFileDescriptor = open(userInputArray[outPipeLocation + 1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    //error and exit if file can't open
    if (*outFileDescriptor < 0)
    {
        perror("Unable to open output file\n");
        return -8888;
    }
    //otherwise, remove '>' symbol and filepath from array,
    //shifting everything over 2 places
    for (int i = outPipeLocation; i < *numArguments - 2; i++)
    {
        userInputArray[i] = userInputArray[i + 2];
    }
    *numArguments -= 2;
    return 0;
}

void startBackgroundProcess(char *userInputArray[], int numArguments)
{
    int childExitMethod = -1337;
    int inPipeLocation = -1337;
    int outPipeLocation = -1337;
    int inFileDescriptor = -1337;
    int outFileDescriptor = -1337;

    pid_t pid = fork();

    switch (pid)
    {
    case (-1):
        perror("could not fork!");
        exit(1);
        break;
    case (0): //child
        //input redirection with dup2()
        findRedirections(userInputArray, numArguments, &inPipeLocation, &outPipeLocation);
        if (inPipeLocation != -1337) //if we found a ">"
        {
            if (openFileForReading(userInputArray, &numArguments, &inFileDescriptor, inPipeLocation) == -8888)
            { //if could not open file, error and exit
                printf("could not open file for reading\n");
                exit(1);
            }
            dup2(3, 0); //if opened, redirect stdin
        }
        else //if there was no redirection for reading, redirect to dev/null
        {
            inFileDescriptor = open("/dev/null", O_RDONLY);
            dup2(inFileDescriptor, 0);
        }
        if (outPipeLocation != -1337)
        {
            if (openFileForWriting(userInputArray, &numArguments, &outFileDescriptor, outPipeLocation) == -8888)
            { //if could not open file, error and exit
                printf("could not open file for writing\n");
                exit(1);
            }
            if (inPipeLocation != -1337) //if we found a "<"
            {
                dup2(4, 1); //files open for both reading and writing
            }
            else
            {
                dup2(3, 1); //only open for writing
            }
        }
        else //if there was no redirection for writing, redirect to dev/null
        {
            outFileDescriptor = open("/dev/null", O_RDONLY);
            dup2(outFileDescriptor, 0);
        }
        //exec()
        userInputArray[numArguments] = NULL;
        userInputArray[numArguments - 1] = NULL; //remove trailing ampersand
        if (execvp(userInputArray[0], userInputArray) == -1)
        {
            printf("command \"%s\" is not a valid command\n", userInputArray[0]);
            fflush(stdout);
            return;
        }
        close(inFileDescriptor);
        close(outFileDescriptor);
        break;
    }
    waitpid(pid, &childExitMethod, WNOHANG);
    printf("background pid is %d\n", pid);
    fflush(stdout);
}

void startForegroudProcess(char *userInputArray[], int numArguments, int *status, bool *signalFlag)
{

    int childExitMethod = -1337;
    int inPipeLocation = -1337;
    int outPipeLocation = -1337;
    int inFileDescriptor = -1337;
    int outFileDescriptor = -1337;

    pid_t pid = fork();

    switch (pid)
    {
    case (-1):
        perror("could not fork!");
        exit(1);
        break;
    case (0): //child
        //input redirection with dup2()
        findRedirections(userInputArray, numArguments, &inPipeLocation, &outPipeLocation);
        if (inPipeLocation != -1337)
        {
            if (openFileForReading(userInputArray, &numArguments, &inFileDescriptor, inPipeLocation) == -8888)
            { //could not open file
                *status = 1;
                *signalFlag = false;
                exit(1);
            }
            dup2(3, 0);
        }
        if (outPipeLocation != -1337)
        {
            if (openFileForWriting(userInputArray, &numArguments, &outFileDescriptor, outPipeLocation) == -8888)
            { //could not open file
                *status = 1;
                *signalFlag = false;
                exit(1);
            }
            if (inPipeLocation != -1337)
            { //files open for both reading and writing
                dup2(4, 1);
            }
            else
            { //only open for writing
                dup2(3, 1);
            }
        }
        //exec()
        userInputArray[numArguments] = NULL;
        if (execvp(userInputArray[0], userInputArray) == -1)
        {
            *status = 1;
            *signalFlag = false;
            printf("command \"%s\" is not a valid command\n", userInputArray[0]);
            fflush(stdout);
            return;
        }
        foregroundPID = getpid();
        printf("%d ", foregroundPID);
        close(inFileDescriptor);
        close(outFileDescriptor);
        break;
    }
    waitpid(pid, &childExitMethod, 0);

    //return exit status value
    if (WIFEXITED(childExitMethod) != 0)
    {
        *signalFlag = false;
        *status = WEXITSTATUS(childExitMethod);
    }
    //return terminating signal value
    else if (WIFSIGNALED(childExitMethod) != 0)
    {
        *signalFlag = true;
        *status = WTERMSIG(childExitMethod);
    }
    foregroundPID = -1;
}

bool executeInput(char *userInputArray[], int numArguments, int *status, bool *signalFlag)
{

    /*** EXIT ***/
    if (strcmp(userInputArray[0], "exit") == 0)
    {
        return true; //set flag to break out of loop in main
    }
    /*** CD ***/
    else if (strcmp(userInputArray[0], "cd") == 0)
    {
        if (userInputArray[1] == NULL)
        {                                                 //change to home directory
            struct passwd *userInfo = getpwuid(getuid()); //get home directory as char *
            chdir(userInfo->pw_dir);
        }
        else
        {
            chdir(userInputArray[1]); //change to specified directory
        }
    }

    /*** STATUS ***/
    else if (strcmp(userInputArray[0], "status") == 0)
    {
        printExitStatus(status, signalFlag);
    }

    /*** OTHER COMMAND ***/
    else
    {       
        //only start in backround if not in foregroundOnly mode and '&' is last arguement
        if (foregroundOnlyMode != true && strcmp(userInputArray[numArguments - 1], "&") == 0)
        {
            startBackgroundProcess(userInputArray, numArguments);
        }
        else
        {   //if in foregroundOnly mode, remove trailing ampersand if present
            if(foregroundOnlyMode == true){
                if(strcmp(userInputArray[numArguments - 1], "&") == 0){
                    userInputArray[numArguments - 1] = "\0";
                    numArguments--;
                }
            }
            startForegroudProcess(userInputArray, numArguments, status, signalFlag);
        }
    }

    findZombies(status, signalFlag);
    return false;
}

// This is the main loop that gets input from the user, parses it,
// passes parsed input to the execution function, then loops again for input
void mainShellLoop()
{
    char prompt[] = ": ";
    char *userInput = NULL;
    char *token = NULL;
    char *userInputArray[512] = {0};
    int numArguments = 0;
    size_t bufferSize = 0;
    int numCharsEntered = -1337;
    int fgExitStatus = 0;
    bool signalFlag = false;
    bool breakFlag = false;

    //main while loop for shell, runs forever until terminated
    while (1)
    {
        while (1)
        { //prompt for input, using getline as per reading 3.3
            write(STDOUT_FILENO, prompt, 2);
            fflush(stdout);
            numCharsEntered = getline(&userInput, &bufferSize, stdin);
            if (numCharsEntered == -1)
            {
                clearerr(stdin);
            }
            else
            {
                userInput[strcspn(userInput, "\n")] = '\0'; //remove trailing newline
                break;
            }
        }
        //if input is empty or a comment line,
        //loop for input again, ignoring everything below
        if (userInput[0] == '\0' || userInput[0] == '#')
        {
            findZombies(&fgExitStatus, &signalFlag);
            continue;
        }

        //token is null when string is empty, so this will loop over entire string
        //saving each space-delimited argument into an array
        token = strtok(userInput, " ");
        while (token != NULL)
        {
            if(strstr(token, "$$") != NULL){ //if we find $$
                //remove $$
                char newString[512];
                memset(newString, '\0', 512);
                memcpy(newString, token, strlen(token) - 2);
                newString[strlen(token) - 2] = '\0';

                //get pid as string
                int pid = getpid();
                char pidString[32];
                memset(pidString, '\0', 32);
                sprintf(pidString, "%d", pid);

                //concat
                strcat(newString, pidString);
                
                token = newString;
            }
            userInputArray[numArguments] = token;
            numArguments++;
            token = strtok(NULL, " ");
        }

        breakFlag = executeInput(userInputArray, numArguments, &fgExitStatus, &signalFlag);

        //free input, reset arrray and counter before next loop
        free(userInput);
        userInput = NULL;
        memset(userInputArray, 0, 512);
        numArguments = 0;

        //break loop if user selected "exit"
        if (breakFlag)
        {
            return;
        }
    }
}

void killForegroundProcess(){

    char * exitMessage = "terminated by signal 2\n";
    int messageSize = 23;
    write(STDOUT_FILENO, exitMessage, messageSize);

    if(foregroundPID != -1){
        kill(foregroundPID, SIGKILL);
        foregroundPID = -1;
    }
    
}

void toggleForegroundOnly(){
    char * exitMessage;
    int messageSize;
    
    if(foregroundOnlyMode == true){
        foregroundOnlyMode = false;
        exitMessage = "Exiting foreground-only mode\n";
        messageSize = 29;
    } else {
        foregroundOnlyMode = true;
        exitMessage = "Entering foreground-only mode (& is now ignored)\n";
        messageSize = 49;
    }

    write(STDOUT_FILENO, exitMessage, messageSize);
}

int main(int argc, char **argv)
{
    // makes SIGINT run killForegroundProcess() instead of default action
    struct sigaction sigintKillForeground = {{0}};
    sigemptyset(&sigintKillForeground.sa_mask);
    sigintKillForeground.sa_flags = SA_RESTART;
    sigintKillForeground.sa_handler = killForegroundProcess; //don't do the default action
    sigaction(SIGINT, &sigintKillForeground, NULL); 

    // makes SIGSTP run toggleForegroundOnly() instead of default action
    struct sigaction sigstpToggleFGonly = {{0}};
    sigemptyset(&sigstpToggleFGonly.sa_mask);
    sigstpToggleFGonly.sa_flags = 0;
    sigstpToggleFGonly.sa_handler = toggleForegroundOnly; //don't do the default action
    sigaction(SIGTSTP, &sigstpToggleFGonly, NULL); 


    // Run command loop.
    mainShellLoop();

    return 0;
}