#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h> //isalpha

int checkValidity(char * filename){
    FILE *file;
    char c;
    char *buffer;

    file = fopen(filename, "r"); //open the file read-only

    if (file == NULL){
        return 1; //could not open file
    }
    while ((c = fgetc(file)) != EOF){
        if(isalpha(c) == 0 && c != ' ' && c != '\n'){
            return 1;
        }
    }
    return 0;
}

long int getCharCount(char * filename)
{
    FILE *file;
    long int fileSize;

    file = fopen(filename, "r"); //open the file read-only

    //find file size, reference: https://stackoverflow.com/a/238609
    fseek(file, 0, SEEK_END); // seek to end of file
    fileSize = ftell(file);   // get current file pointer
    rewind(file);

    fclose(file);

    return fileSize;
}

long int sendFile(char * filename, int socketFD)
{
    int bytesRead, bytesSent;
    FILE *file;
    long int fileSize;
    char *buffer;
    char *fileSizeString;

    file = fopen(filename, "r"); //open the file read-only

    //find file size, reference: https://stackoverflow.com/a/238609
    fseek(file, 0, SEEK_END); // seek to end of file
    fileSize = ftell(file);   // get current file pointer
    rewind(file);             // seek back to beginning of file

    //copy fileSize to string
    fileSizeString = malloc(sizeof(fileSize));
    memset(fileSizeString, '\0', strlen(fileSizeString)+1);
    sprintf(fileSizeString, "%d", fileSize);

    //create buffer to hold file
    buffer = malloc(sizeof(char) * (fileSize));

    //read file into buffer and close the file
    bytesRead = fread(buffer, sizeof(char), fileSize, file);
    fclose(file);
    strcat(buffer, "\0"); //append EOF to end of buffer

    //send filesize before sending file
    send(socketFD, fileSizeString, strlen(fileSizeString), 0);

    //server will reply with "OK" if size was received
    recv(socketFD, fileSizeString, 3, 0);

    if(strcmp(fileSizeString, "OK") == 0)
    {
        send(socketFD, buffer, strlen(buffer), 0);
    }

    free(buffer);
    free(fileSizeString);
    return fileSize;
}

int main(int argc, char *argv[])
{
    long int socketFD, portNumber, charsWritten, charsRead, fileSize;
    struct sockaddr_in serverAddress;
    struct hostent *serverHostInfo;
    char buffer[81920];
    memset(buffer, '\0', 81920);
    char * message;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: otp_enc <plain text file> <key text file> <port>\n");
        exit(1);
    }

    if(getCharCount(argv[1]) > getCharCount(argv[2])){
        fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
        exit(1);
    }
    if(checkValidity(argv[1]) == 1){
        fprintf(stderr, "otp_enc error: input contains bad characters");
        exit(1);
    }    

    // Set up the server address struct
    memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[3]);                                  // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET;                          // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber);                  // Store the port number
    serverHostInfo = gethostbyname("os1");                       // Convert the machine name into a special form of address
    if (serverHostInfo == NULL)
    {
        fprintf(stderr, "CLIENT: ERROR, no such host\n");
        exit(1);
    }
    memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)serverHostInfo->h_addr_list[0], serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0)
    {
        fprintf(stderr, "CLIENT: ERROR opening socket");
        exit(1);
    }

    // Connect to server
    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    { // Connect socket to address
        fprintf(stderr, "CLIENT: ERROR connecting to server");
        exit(1);
    }

    // send initial message to server
    charsWritten = send(socketFD, "I am otp_enc", 13, 0);
    if (charsWritten < 0)
    {
        fprintf(stderr, "CLIENT: ERROR writing to socket");
        exit(1);
    }

    // get response from server
    charsRead = recv(socketFD, buffer, 255, 0);
    if (strcmp(buffer, "I am otp_enc_d") != 0)
    {
        fprintf(stderr, "ERROR not connected to otp_enc_d");
        exit(2);
    }
    else //send plaintext if valid
    {
        fileSize = sendFile(argv[1], socketFD) - 1;
        sendFile(argv[2], socketFD);

        //recieve encrypted message & print to stdout
        message = malloc(sizeof(char) * fileSize + 1);
        memset(message, '\0', sizeof(message));

        while(fileSize > 0){
            charsRead = recv(socketFD, buffer, sizeof(buffer), 0);
            strcat(message, buffer);
            memset(buffer, '\0', 81920);
            fileSize -= charsRead;
        }
        printf("%s\n", message);
        // free(message);
    }
}