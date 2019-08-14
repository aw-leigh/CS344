/* Andrew Wilson (wilsoan6) CS 372 Project 2
 * Simple FTP utility using socket programming
 * Last modified: 8/7/2019
 */

#define _POSIX_C_SOURCE 200112L //https://stackoverflow.com/a/37545256

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> //For Sockets
#include <sys/stat.h>   //for file controls
#include <fcntl.h>      //for file controls
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)
#include <dirent.h>     //for directory listing

// Pre: needs a file descriptor and a port number to bind to
// Post: listening on provided port
// The following function is taken mostly directly from Beej's guide
void setupServer(int *fd, char *port)
{

    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    getaddrinfo(NULL, port, &hints, &res);

    *fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (*fd < 0)
    {
        fprintf(stderr, "could not create socket");
        exit(1);
    }
    if (bind(*fd, res->ai_addr, res->ai_addrlen) < 0)
    {
        fprintf(stderr, "could not bind");
        exit(1);
    }
    if (listen(*fd, 1) < 0)
    {
        fprintf(stderr, "could not listen");
        exit(1);
    }
}
// connects to remote server as client, adapted from the previous project
// Pre: needs a hostname, port, and file descriptor for a remote server
// Post: connected to remote server
void setupClientConnection(char *host, char *port, int *fd)
{

    struct sockaddr_in chatServer; //main socket to store server details

    //create an create an AF_INET (IPv4), STREAM socket (TCP)
    *fd = socket(AF_INET, SOCK_STREAM, 0);

    chatServer.sin_family = AF_INET;
    chatServer.sin_port = htons(atoi(port)); //user specified port

    inet_pton(AF_INET, host, &chatServer.sin_addr); //user specified host name
    connect(*fd, (struct sockaddr *)&chatServer, sizeof(chatServer));
}

// adapted from: https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
// Pre: needs a large string to save directory contents into
// Post: directory contents are saved to the provided string, ignoring "." and "..", delimited with "\n"
void printDirectoryContentsToString(char contents[])
{
    struct dirent *de; // Pointer for directory entry
    char name[64];

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(".");

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
        exit(1);
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html for readdir()
    while ((de = readdir(dr)) != NULL)
    {
        if (!(strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0))
        {
            sprintf(name, "%s\n", de->d_name);
            strcat(contents, name);
        }
    }
    //printf("%s", contents);
    closedir(dr);
}

// sends contents of current directory as a \n delimited string to remote host, then closes the connection
// Pre: needs a string array with remote host in array[4] and remote port in array[1]
// Post: string of directory contents sent to remote
void sendDirectoryContents(char *commands[], char directoryContents[])
{
    int dataConnectionFD, bytesSent;

    setupClientConnection(commands[4], commands[1], &dataConnectionFD);

    bytesSent = send(dataConnectionFD, directoryContents, strlen(directoryContents), 0);
    close(dataConnectionFD);
}

// Opens connection to remote, sends a file as a stream, then closes connection
// Pre: file must exist in current directory
// Post: file is sent as stream to remote
void sendFile(char *commands[])
{
    int dataConnectionFD, bytesRead, bytesSent;
    FILE *file;
    long int fileSize;
    char *buffer;

    setupClientConnection(commands[4], commands[2], &dataConnectionFD);

    file = fopen(commands[1], "r"); //open the file read-only

    //find file size, reference: https://stackoverflow.com/a/238609
    fseek(file, 0, SEEK_END); // seek to end of file
    fileSize = ftell(file);   // get current file pointer
    rewind(file);             // seek back to beginning of file

    //create buffer to hold file
    buffer = malloc(sizeof(char) * (fileSize + 1));

    //read file into buffer and close the file
    bytesRead = fread(buffer, sizeof(char), fileSize, file);
    fclose(file);
    strcat(buffer, "\0"); //append EOF to end of buffer

    //ensure we send all of any large files
    while (fileSize > 0)
    {
        bytesSent = send(dataConnectionFD, buffer, strlen(buffer), 0);
        if (bytesSent < 0)
        {
            fprintf(stderr, "Error in writing\n");
            return;
        }
        fileSize -= bytesSent;
    }

    printf("File sent\n");
    close(dataConnectionFD);
    free(buffer);
}

// opens connection to remote, sends error message, and closes connection
void sendError(char *host, char *port, char errorCode[])
{
    int dataConnectionFD;
    char *errorMessage = errorCode;

    setupClientConnection(host, port, &dataConnectionFD);
    send(dataConnectionFD, errorMessage, strlen(errorMessage), 0);
    close(dataConnectionFD);
}

// main logic for handling requests. It will parse input and send a directory listing,
// file, or error to client.
// Pre: Need control connection with client established
// Post: Connection has been made, data sent, and connection closed.
void handleRequest(int controlConnectionFD, char *clientHost, char directoryContents[])
{
    char buffer[1028];
    char *commands[5]; //used to store commands from client
    int numCommands = 0;
    char *token;

    memset(buffer, '\0', sizeof(buffer));
    memset(commands, '\0', sizeof(commands));

    //recieve commands from client into buffer
    recv(controlConnectionFD, buffer, sizeof(buffer), 0);

    //token is null when string is empty, so this will loop over entire string
    //saving each space-delimited argument into the array
    token = strtok(buffer, " ");
    while (token != NULL)
    {
        commands[numCommands] = token;
        numCommands++;
        token = strtok(NULL, " ");
    }
    commands[4] = clientHost; //add hostname to last place in commands array

    if (strcmp(commands[0], "-l") == 0)
    { //commands should be "-l <port>"
        printf("List directory requested on port %s\n", commands[1]);
        printf("Sending directory contents to %s:%s\n", commands[4], commands[1]);
        sendDirectoryContents(commands, directoryContents);
    }
    else if (strcmp(commands[0], "-g") == 0)
    { //commands should be "-g <filename> <port>"
        printf("File \"%s\" requested on port %s\n", commands[1], commands[2]);

        // returns NULL if substring isn't found
        if (strstr(directoryContents, commands[1]) == NULL)
        {
            printf("File not found. Sending error to client.\n");
            if (commands[2] == NULL)
            {
                sendError(commands[4], commands[1], "-1");
            }
            else
            {
                sendError(commands[4], commands[2], "-1");
            }
        }
        else
        {
            printf("Sending \"%s\" to %s:%s\n", commands[1], commands[4], commands[2]);
            sendFile(commands);
        }
    }
    else //command not recognized
    {
        printf("Command not recognized. Sending error to client.\n");
        if (commands[2] == NULL)
            {
                sendError(commands[4], commands[1], "-2");
            }
            else
            {
                sendError(commands[4], commands[2], "-2");
            }
    }
    close(controlConnectionFD);
}

int main(int argc, char *argv[])
{

    int serverFD, controlConnectionFD;      //file descriptor that is used to idenfiy the socket
    struct sockaddr_storage controlAddress; //for storing connection info
    socklen_t ctrlAdrSize;
    char clientHost[128];
    char clientPort[16];
    char directoryContents[1000]; //to store filenames in current directory
    char *token;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: $ /ftserver <port>\n");
        exit(1);
    }

    setupServer(&serverFD, argv[1]);
    printf("Server open on port %s\n", argv[1]);

    // this used to be a subfunction of handleRequest, and I used dynamic lookup instead of printing to string,
    // but it was causing file transfers to hang for some reason.
    // Everything works when it's in main, so in main it will remain
    memset(directoryContents, '\0', sizeof(directoryContents));
    printDirectoryContentsToString(directoryContents);

    //main server loop
    while (1)
    {
        //accept connection
        ctrlAdrSize = sizeof(controlAddress);
        controlConnectionFD = accept(serverFD, (struct sockaddr *)&controlAddress, &ctrlAdrSize);

        //extract & print the hostname
        getnameinfo((struct sockaddr *)&controlAddress, ctrlAdrSize, clientHost, 128, clientPort, 16, 0);
        token = strtok(clientHost, ".");
        strcpy(clientHost, token);
        printf("Connection from %s\n", clientHost);

        handleRequest(controlConnectionFD, clientHost, directoryContents);
    }
}