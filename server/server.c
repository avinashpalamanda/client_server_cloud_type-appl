#include "server_helper.h"
#include "server.h"
FILE *logfile;

int main(){
  system("clear");
  logfile = fopen("logfile_t.txt","w");
  fprintf(logfile, "Server Started\n");
  fflush(logfile);

  struct sockaddr_in server , client;
  socklen_t clilen;

  fd_set master;
  fd_set read_fds;
  FD_ZERO(&master);
  FD_ZERO(&read_fds);

  int sockFD, newFD;
  int yes = 1;
  int message;

  // get us a socket and bind it
  sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
      fprintf(logfile,"Error in Creating a Socket\n");
      fflush(logfile);
      exit(0);
    }
  // lose the pesky "address already in use" error message
  setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  //Prepare the sockaddr_in structure
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons( 3000 );

  if (bind(sockFD, (struct sockaddr *)&server , sizeof(server)) < 0){
    close(sockFD);
    fprintf(logfile,"Error in Binding Socket\n");
    fflush(logfile);
    exit(0);
  }

  listen(sockFD, 10);

  FD_SET(sockFD, &master);
  FD_SET(0, &master);
  // keep track of the biggest file descriptor
  int fdmax = sockFD;

  while(1){
    read_fds = master; // copy it
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      fprintf(logfile,"Error in select call\n");
      fflush(logfile);
      exit(0);
    }

    for (int i = 0;i <= fdmax; i++){
      if (FD_ISSET(i, &read_fds)) {
        if (i == sockFD){
          clilen = sizeof(client);
          newFD = accept(sockFD, (struct sockaddr *)&client, &clilen);
          if (newFD < 0) {
            fprintf(logfile,"ERROR on accept %s\n",address(i));
            fflush(logfile);
            exit(1);
          }
          else{
            fprintf(logfile, "Connection from %s accepted\n", inet_ntoa(client.sin_addr));
            fflush(logfile);
            FD_SET(newFD, &master);
            if(fdmax < newFD){
              fdmax = newFD;
            }
          }
        }
        else if (i == 0) {
          char command[128];
          scanf("%s", command);
          if (strcmp(command, "exit") == 0) {
            fprintf(logfile, "Exiting from Server\n");
            fflush(logfile);
            close(sockFD);
            return 0;
          }
        }
        else{
          int nbytes = recv(i,&message,1*sizeof(int),0);
          if( nbytes <= 0) {
            if(nbytes == 0) {
              fprintf(logfile,"socket fd %s Left server\n",address(i));
              fflush(logfile);
            }
            else {
              fprintf(logfile,"Error in recieving from socket fd %s \n",address(i));
              fflush(logfile);
            }
            FD_CLR(i,&master);
          }
          else {
           if (message == 1) {
             fprintf(logfile,"Sorting Request from FD %s \n",address(i));
             fflush(logfile);
             pthread_t tid;
             pthread_create(&tid, NULL, sorting,(void*)&i);
             pthread_join(tid, NULL);
          }
          if (message == 3) {
            fprintf(logfile,"Fibonacci Request from FD %s\n",address(i));
            fflush(logfile);
            pthread_t tid;
            pthread_create(&tid, NULL, fibonacci,(void*)&i);
            pthread_join(tid, NULL);
          }
          if (message == 2) {
            fprintf(logfile,"Random Number Request from FD %s\n",address(i));
            fflush(logfile);
            pthread_t tid;
            pthread_create(&tid, NULL, randomNumber,(void*)&i);
            pthread_join(tid, NULL);
          }
          fflush(stdout);
          }
        }
      }
    }
  }
}
