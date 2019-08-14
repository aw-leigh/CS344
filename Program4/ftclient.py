#!/usr/bin/env python

# Andrew Wilson (wilsoan6) CS 372 Project 2
# Simple FTP utility using socket programming
# Last modified: 8/7/2019

# references: https://www.binarytides.com/python-socket-programming-tutorial/
# references: https://shakeelosmani.wordpress.com/2015/04/13/python-3-socket-programming-example/

import socket   # sockets
import sys      # exit, write
import os.path  # check directory contents

def overwriteFileCheck():
    if(len(sys.argv) == 6): # if we are requesting a file with -g
        if os.path.exists("./" + sys.argv[4]): # if a file with that name exists in directory
            reply = raw_input("Overwrite "+ sys.argv[4] + "? (y/n)")
            if(reply == "n" or reply == "N"):
                exit(0)

# Sets up TCP socket for recieving data from server
# Pre: argv1 must be a port number
# Post: socket is set up
def setupDataPort():
    HOST = ''                       # all available interfaces

    if len(sys.argv) == 5:          # user-specified port
        PORT = int(sys.argv[4])
    else:
        PORT = int(sys.argv[5])

    # create an AF_INET (IPv4), STREAM socket (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    return s


# Sets up TCP socket to connect to remote server
# Pre: argv1 must be a port number
# Post: socket is set up
def connectToServer():

    # create an AF_INET (IPv4), STREAM socket (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    host = sys.argv[1]
    port = int(sys.argv[2])

    try: #get IP address of host
	remote_ip = socket.gethostbyname(host) 

    except socket.gaierror:
        #could not get IP address
        print 'Could not get hostname IP address ;('
        sys.exit()

    #Connect to remote server
    s.connect((remote_ip , port))

    return s

# Connects to remote host and sends message based on cli arguments
# Pre: CLI arguments must be as specified
# Post: Message sent to server
def makeRequest():
    if(len(sys.argv) == 5): #-l
        message = sys.argv[3] + " " + sys.argv[4]
    else: #-g
        message = sys.argv[3] + " " + sys.argv[4] + " " + sys.argv[5]
    
    serverSocket.sendall(message)

# Recieves data from remote host
# Pre: Must have connection with remote host
# Post: Data from remote host is printed to stdout or saved to file depending on command
def recieveData():
    data = connection.recv(1024)

    if (data == "-1"):
        print(sys.argv[1] + ":" + sys.argv[2] + "says: " + "\nError: File not found\n")
    elif (data == "-2"):
        print(sys.argv[1] + ":" + sys.argv[2] + "says: " + "\nError: Command not recognized\n")
    elif(len(sys.argv) == 5): #-l
        sys.stdout.write(data) # print directory contents
    else: #-d
        print("Receiving file data") # save file

        f = open(sys.argv[4],"w")
        while (data):
                f.write(data)
                data = connection.recv(1024)
    return data

####################
### MAIN PROGRAM ###
####################

if (len(sys.argv) < 5 or len(sys.argv) > 6):
    print(len(sys.argv))
    print("Use: ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> (<FILENAME>) <DATA_PORT>")
    exit(1)

# check if file exists and prompt to overwrite
overwriteFileCheck()

dataSocket = setupDataPort()
serverSocket = connectToServer()

# send request to server
makeRequest()

# setup data port to revieve reply
while True:
    dataSocket.listen(1)  # listen for 1 processes at a time only
    connection, address = dataSocket.accept()

    recieveData()
    connection.close()
    break

serverSocket.close()
dataSocket.close()
exit(0)