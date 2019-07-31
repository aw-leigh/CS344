#define _GNU_SOURCE //because I was getting an implicit declaration error with g++, reference: https://stackoverflow.com/a/8494003
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdbool.h> //for bools
#include <pwd.h> //for finding HOME directory


bool executeInput(char* userInputArray[], int numArguments){
    if(strcmp(userInputArray[0], "exit") == 0){
        //kill all children here

        return true; //set flag to break out of loop in main
    } 
    else if (strcmp(userInputArray[0], "cd") == 0){
        if(userInputArray[1] == NULL){ //change to home directory
            struct passwd * userInfo = getpwuid(getuid()); //get home directory as char *
            chdir(userInfo->pw_dir);
        } else {
            chdir(userInputArray[1]); //change to specified directory
        }
    }
    else if (strcmp(userInputArray[0], "status") == 0){
        //report status here
    } 
    else {//non-built in function
        if(strcmp(userInputArray[numArguments], "&") == 0){
            //background execution
        } else{
            //foreground execution
        }
    }
    return false;
}

// This is the main loop that gets input from the user, parses it,
// passes parsed input to the execution function, then loops again for input
void mainShellLoop(){
    char prompt[] = ": ";
    char* userInput = NULL;
    char* token = NULL;
    char* userInputArray[512] = {0};
    int numArguments = 0;
    size_t bufferSize = 0;
    int numCharsEntered = -1337;
    bool breakFlag = false;

    //main while loop for shell, runs forever until terminated
    while(1){
        while(1){ //prompt for input, using getline as per reading 3.3
            write(STDOUT_FILENO, prompt, 2);
            numCharsEntered = getline(&userInput, &bufferSize, stdin);
            if (numCharsEntered == -1)
                clearerr(stdin);
            else
                userInput[strcspn(userInput, "\n")] = '\0'; //remove trailing newline
                break;
        }
        //if input is empty or a comment line,
        //loop for input again, ignoring everything below
        if(userInput[0] == '\0'|| userInput[0] == '#'){
            continue;
        }
        
        //token is null when string is empty, so this will loop over entire string
        //saving each space-delimited argument into an array
        token = strtok(userInput, " ");
        while(token != NULL){
            userInputArray[numArguments] = token;
            numArguments++;
            token = strtok(NULL, " ");
        }

        breakFlag = executeInput(userInputArray, numArguments);
        
        // print input, for debugging
        // for(int i = 0; i < numArguments; i++){
        //     printf("%d: %s\n ",i, userInputArray[i]);
        // }

        //free input, reset arrray and counter before next loop 
        free(userInput);
        userInput = NULL;
        memset(userInputArray, 0, 512);
        numArguments = 0;
        
        //break loop if user selected "exit"
        if(breakFlag){
            return;
        }
    }
}


int main(int argc, char **argv)
{
    // Run command loop.
    mainShellLoop();

    // Perform any shutdown/cleanup.

    return 0;
}