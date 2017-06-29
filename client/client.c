#include "client_helper.h"
#include "client.h"

int main(int argc, char *argv[]){
  system("clear");

  if (argc!=2) {
    printf("Please inter the ip address Example : ./client 127.0.0.1");
    return 0;
  }

  struct sockaddr_in server;
  fd_set master;
  fd_set read_fds;
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  int sockFD;
  int yes = 1;

  // get us a socket and bind it
  sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
      printf("Error in Creating a Socket\n");
    }

   server.sin_addr.s_addr = inet_addr(argv[1]);
   server.sin_family = AF_INET;
   server.sin_port = htons( 3000 );

   //Connect to remote server
   if (connect(sockFD, (struct sockaddr *)&server , sizeof(server)) < 0) {
     printf("connect failed. Error");
     return 1;
   }

   FD_SET(0, &master);
   FD_SET(sockFD, &master);
   int fdmax = sockFD;

   usage();

   while(1){
     read_fds = master; // copyit
     if (select(1, &read_fds, NULL, NULL, NULL) == -1) {
       printf("select");
       exit(4);
     }
     for (int i = 0; i <= fdmax; i++){
       if (FD_ISSET(i, &read_fds)) {
         if (i == 0){
           char command[128];
           scanf("%s", command);
           if (strcmp(command, "sort") == 0) {
             if (sorting(sockFD))
              printf("Error in Sorting\n");
           }
           else if (strcmp(command,"fibonacci") == 0) {
             if (fibonacci(sockFD))
              printf("Error in Fibonacci\n");
           }
           else if (strcmp(command,"random") == 0) {
             if(randomNumber(sockFD))
              printf("Error in Random\n");
           }
           else if(strcmp(command,"exit")==0){
            close(sockFD);
            return 0;
           }
           else {
             system("clear");
             usage();
           }
         }
       }
     }
   }
 }
