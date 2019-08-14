#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int sendFile(char * filename, int socketFD)
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
    fileSizeString = malloc(sizeof(fileSize) + 1);
    memset(fileSizeString, '\0', strlen(fileSizeString));
    sprintf(fileSizeString, "%d", fileSize);
    printf("File size: %d\n", fileSize);

    //create buffer to hold file
    buffer = malloc(sizeof(char) * (fileSize + 1));

    //read file into buffer and close the file
    bytesRead = fread(buffer, sizeof(char), fileSize, file);
    fclose(file);
    strcat(buffer, "\0"); //append EOF to end of buffer

    printf("Buffer:\n%s\n", buffer);

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
    return 0;
}

int main(int argc, char *argv[])
{
    int socketFD, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    struct hostent *serverHostInfo;
    char buffer[256];
    memset(buffer, '\0', 256);

    if (argc != 4)
    {
        fprintf(stderr, "Usage: otp_end <plain text file> <key text file> <port>\n");
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
    else //send plaintext
    {
        sendFile(argv[1], socketFD);
        sendFile(argv[2], socketFD);
    }
}