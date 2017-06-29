#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

int sorting(int sockFD){
  int type = 1;
  if (send(sockFD , &type, 1*sizeof(int) , 0) < 0 ) {
    printf("Send failed\n");
    return 1;
  }

  int sort_size = 10;
  scanf("%d", &sort_size);
  if(send(sockFD , &sort_size, 1*sizeof(int) , 0) < 0) {
    printf("Send failed");
    return 1;
  }

  int list[sort_size];
  for (int i = 0; i < sort_size; i++) {
    scanf("%d", &list[i]);
  }
  if (send(sockFD , &list, sort_size*sizeof( int ) , 0) < 0) {
    printf("Send failed");
    return 1;
  }

  int nbytes = recv(sockFD,&list,sort_size*sizeof(int),0);
  if (nbytes <= 0) {
    printf( "Error in recieving %d\n",nbytes );
  }

  char result[sort_size];
  int index = 0;
  index += sprintf( &result[index] ,"Sorted List : " );
  for (int i = 0; i < sort_size; i++) {
    index += sprintf( &result[index], "%d ", list[i] );
  }

  printf("%s\n", result);
  fflush(stdout);
  return 0;
}

int randomNumber(int sockFD) {
  int type = 2;
  if (send(sockFD , &type, 1*sizeof(int) , 0 ) < 0) {
    printf("Send failed");
    return 1;
  }

  int x[2];
  scanf("%d %d", &x[0], &x[1]);
  if (send( sockFD , &x, 2*sizeof(int) , 0 ) < 0) {
    printf("Send failed");
    return 1;
  }

  int random_number;
  int nbytes = recv( sockFD, &random_number, 1*sizeof(int), 0 );
  if (nbytes <= 0) {
    printf("Error in recieving %d\n",nbytes);
  }

  char result[128];
  int index = 0;
  index += sprintf(&result[index], "Random Number : %d ", random_number);

  printf("%s\n", result);
  fflush(stdout);
  return 0;
}

int fibonacci(int sockFD){
  int type = 3;
  if( send(sockFD, &type, 1*sizeof(int), 0) < 0) {
    printf("Send failed");
    return 1;
  }

  int x;
  scanf("%d", &x);
  if (send(sockFD, &x, 1*sizeof(int), 0) < 0) {
    printf("Send failed");
    return 1;
  }

  int fib;
  int nbytes = recv(sockFD, &fib, 1*sizeof(int), 0);
  if( nbytes <= 0){
    printf("Error in recieving %d\n", nbytes);
  }

  char result[128];
  int index = 0;
  index += sprintf(&result[index], "Fibonacci Number : %d ", fib);

  printf("%s\n", result);
  fflush(stdout);
  return 0;
}

int usage(){
  printf("            Usage        \n");
  printf("Sorting       -  sort      [Size of List] [List Values]\n");
  printf("Random Number -  random    [Start] [End]\n");
  printf("Fibonacci     -  fibonacci [Number]\n");
  printf("Exit          -  exit\n");
  fflush(stdout);
  return 0;
}
