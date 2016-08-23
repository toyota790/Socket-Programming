/*
 Computer Networks - Assignment 1
 Socket Programming - Client Side
 Date:2015/10/21
 Student ID:M10415010
 Name:William Luo
 Language:C
 OS:Unix/Linux (MAC OSX)
 IDE:XCode
 */

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define portnum 8888  //define the port number

int main(int argc , char *argv[])
{
    
    int sock;
    char IP_address[100];
    struct sockaddr_in server;
    char user_input[6000]="" , server_reply[6000]="";
    
    //Prompt the user to get the server IP address
    printf("Please enter the server IP address: ");
    gets(IP_address);
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket.");
    }
    puts("Socket created.");
    
    //Set up the protocol type, IP Address and port number
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_address);
    server.sin_port = htons(portnum);
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    //Display the connect successfully message and operating instructions
    puts("Connected.");
    puts("Please enter the two operands with an operator. Each operand is a floating point number with a precision of two digits after the decimal point. The operator could be +, - , * or /. You can input Q to quit this program. And here is the input example:12.34+56.78 .\n");
    
    //Keep communicate with server
    while(1)
    {
        //Initialize the message and server_reply
        memset(user_input, 0, sizeof user_input);
        memset(server_reply, 0, sizeof server_reply);
        puts("Please enter your input: ");
        gets(user_input);
        
        //If user enter the Q, then exit the program
        if(!strcmp(user_input, "Q")) {
            break;
        }
        //Send some data
        if( send(sock , user_input , strlen(user_input) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        
        //Receive a reply from the server
        if( recv(sock , server_reply , 6000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        
        //Print the server response result
        puts("Server reply :");
        puts(server_reply);
    }
    
    //Close the socket
    close(sock);
    puts("Close the socket.\nExit the program.");
    return 0;
}