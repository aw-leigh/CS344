#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

char addKeyToPlainText(char keyChar, char textChar)
{
    int result;

    keyChar -= 65; //this makes A = 0, B = 1, ...
    textChar -= 65;

    if (keyChar < 0)
    { // ' ' is 26
        keyChar = 26;
    }
    if (textChar < 0)
    {
        textChar = 26;
    }

    result = ((int)keyChar + (int)textChar) % 27 + 65;
    if (result == 91)
    {
        result = 32; //ASCII for ' '
    }

    return (char)result;
}

int recieveFile(int socketFD, char **outputString)
{
    int bytesRead;
    long int fileSize;
    char fileSizeString[10];

    //recieve filesize
    bytesRead = recv(socketFD, fileSizeString, sizeof(fileSizeString), 0);
    if (bytesRead < 0)
    {
        fprintf(stderr, "Could not recieve file size\n");
        return 1;
    }

    //malloc filesize'd buffer
    fileSize = atoi(fileSizeString);
    *outputString = malloc(sizeof(char) * fileSize);

    send(socketFD, "OK", 3, 0);

    //read string into buffer
    while (fileSize > 0)
    {
        bytesRead = recv(socketFD, *outputString, fileSize, 0);
        fileSize -= bytesRead;
    }

    fileSize = atoi(fileSizeString);
    return fileSize;
}

int main(int argc, char *argv[])
{
    int listenSocketFD, establishedConnectionFD, portNumber, charsRead, i;
    long int fileSize;
    socklen_t sizeOfClientInfo;
    struct sockaddr_in serverAddress, clientAddress;
    pid_t pid;
    int childExitMethod = -1337;
    char buffer[256];
    memset(buffer, '\0', 256);
    char *plainText;
    char *keyText;
    char *encryptedMessage;
    char textChar;
    char keyChar;

    //ensure there are three arguments, and that the third is "&"
    //so nonnumeric port input causes undefined behaviour
    if (argc < 2)
    {
        fprintf(stderr, "Usage: otp_enc_d <port> &\n");
        exit(1);
    }

    // Set up the address struct for this process (the server)
    memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[1]);                                  // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET;                          // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber);                  // Store the port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;                  // Any address is allowed for connection to this process

    // Set up the socket
    listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (listenSocketFD < 0)
    {
        fprintf(stderr, "ERROR opening socket");
        exit(1);
    }

    // Enable the socket to begin listening
    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    { // Connect socket to port
        fprintf(stderr, "ERROR on binding (try a different port)");
        exit(1);
    }
    if (listen(listenSocketFD, 5) == -1)
    { // Flip the socket on - it can now receive up to 5 connections
        fprintf(stderr, "ERROR listening");
        exit(1);
    }

    //main server loop
    while (1)
    {
        sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
        establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
        if (establishedConnectionFD < 0)
        {
            fprintf(stderr, "ERROR on accept");
            continue;
        }

        //fork a new process for each connection
        pid = fork();

        if (pid == -1)
        {
            fprintf(stderr, "ERROR: Could not fork!");
        }
        else if (pid == 0) //child, do work
        { 
            // check to make sure it is communicating with otp_enc
            charsRead = recv(establishedConnectionFD, buffer, 255, 0); // Read the client's message from the socket
            if (charsRead < 0)
            {
                fprintf(stderr, "ERROR reading from socket");
                break;
            }
            if (strcmp(buffer, "I am otp_enc") != 0)
            {
                fprintf(stderr, "ERROR not connected to otp_enc\n");
                break;
            }
            else
            {
                charsRead = send(establishedConnectionFD, "I am otp_enc_d", 15, 0); // Send success back

                //recieve plaintext and grab size of message to encrypt
                fileSize = recieveFile(establishedConnectionFD, &plainText);
                plainText[strcspn(plainText, "\n")] = '\0'; // Remove the trailing \n

                // printf("ENCd filesize: %d\n", fileSize);
                // printf("ENCd strlen p: %d\n", strlen(plainText));
                // printf("%s\n", plainText);

                //recieve key
                recieveFile(establishedConnectionFD, &keyText);

                //encrypt message
                encryptedMessage = malloc(fileSize);
                memset(encryptedMessage, '\0', fileSize);

                for (i = 0; i < strlen(plainText); i++)
                {
                    textChar = plainText[i];
                    keyChar = keyText[i];
                    encryptedMessage[i] = addKeyToPlainText(keyChar, textChar);
                }

                //send encrypted message
                // printf("ENCd strlen e: %d\n", strlen(encryptedMessage));
                // printf("%s\n", encryptedMessage);
                send(establishedConnectionFD, encryptedMessage, strlen(encryptedMessage), 0);

                free(encryptedMessage);
                encryptedMessage = NULL;
                free(plainText);
                plainText = NULL;
                free(keyText);
                keyText = NULL;
            }
        }
        else if (pid > 0){

        //wait until child process is done, then close the connection
        waitpid(pid, &childExitMethod, 0);
        close(establishedConnectionFD);
        }
    }

    close(listenSocketFD);
    return 0;
}