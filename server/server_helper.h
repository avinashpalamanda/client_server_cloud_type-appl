#ifndef SERVER_HELPER_
#define SERVER_HELPER_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

void* randomNumber(void* x_sockFD);
void* fibonacci(void* x_sockFD);
void* sorting(void* x_sockFD);
char* address(int sockFD);

#endif
