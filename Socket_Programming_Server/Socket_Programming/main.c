/*
 Computer Networks - Assignment 1
 Socket Programming - Server Side
 Date:2015/10/21
 Student ID:M10415010
 Name:William Luo
 Language:C
 OS:Unix/Linux(MAC OSX)
 IDE:XCode
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>

#define portnum 8888  //Define the port number
#define IP_Address "127.0.0.1" //Define the IP Address

//Declare the function calculate
void calculate(char , char *, char *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[6000];
    
    //Create socket using IP V4 protocol
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    
    //Display error message
    if (socket_desc == -1)
    {
        printf("Could not create socket.");
    }
    puts("Socket created.");
    
    //Set up the protocol type, IP Address and port number
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_Address);
    server.sin_port = htons(portnum);
    
    //Associates a local address with a socket
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //Display the error message
        perror("Error! Bind failed.");
        return 1;
    }
    puts("Bind done.");
    
    //Listen
    listen(socket_desc , 3);
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    
    //Accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Accept failed.");
        return 1;
    }
    puts("Connection accepted.");
    
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 6000 , 0)) > 0 )
    {
        char result[200];
        
        if(strlen(client_message)>13) {
            strcpy(result , "Your input is incorrect. Please make sure each operand is a floating number with a precision of two digits afeter the decimal point. Thanks!\n");
        }
        else{
            //Determie which operator is
            if(strstr(client_message, "+")>0) {
                calculate('+', client_message, result);
            }
            else if(strstr(client_message, "-")>0) {
                calculate('-', client_message, result);
            }
            else if(strstr(client_message, "*")>0) {
                calculate('*', client_message, result);
            }
            else if (strstr(client_message, "/")>0) {
                calculate('/', client_message, result);
            }
            else {
                strcpy(result , "Your input is incorrect. Please check it. Thanks!\n");
            }
        }
        //Clear client_message
        memset(client_message, 0 , sizeof(client_message));
        //Send the result for client
        write(client_sock , result, strlen(result));
        
    }
    
    if(read_size == 0) {
        puts("Client disconnected.");
        fflush(stdout);
    }
    else if(read_size == -1) {
        perror("Recv failed.");
    }
    
    return 0;
}

//Function calculate: Calculate the answer
void calculate(char op, char message[6000], char *result)
{
    //Split the operands from message
    float num1 = atof(strtok(message, &op));
    float num2 = atof(strtok(NULL, &op));
    float temp;
    switch (op) {
        case '+':
            temp = num1 + num2;
            break;
        case '-':
            temp = num1 - num2;
            break;
        case '*':
            temp = num1 * num2;
            break;
        case '/':
            temp = num1 / num2;
            break;
        default:
            break;
    }
    snprintf(result, 200, "%s%.02f", "Answer: ", temp); //Convert the float to string
}