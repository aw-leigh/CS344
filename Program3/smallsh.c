#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 

void mainShellLoop(){
    char prompt[] = ": ";
    char* userInput = NULL;
    char* token = NULL;
    char* userInputArray[512] = {0};
    int numArguments = 0;
    size_t bufferSize = 0;
    int numCharsEntered = -1337;

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

        //if not empty or comment, break input into array
        token = strtok(userInput, " ");

        while(token != NULL){
            userInputArray[numArguments] = token;
            numArguments++;
            token = strtok(NULL, " ");
        }

        for(int i = 0; i < numArguments; i++){
            printf("%d: %s\n ",i, userInputArray[i]);
        }

        //free input, reset arrray and counter before next loop 
        free(userInput);
        userInput = NULL;
        memset(userInputArray, 0, 512);
        numArguments = 0;
    }
    

}


int main(int argc, char **argv)
{
    // Run command loop.
    mainShellLoop();

    // Perform any shutdown/cleanup.

    return 0;
}