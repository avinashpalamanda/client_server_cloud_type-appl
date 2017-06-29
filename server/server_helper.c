#include "server_helper.h"
FILE *logfile;
pthread_mutex_t mutex;

void* randomNumber(void* x_sockFD) {

  int sockFD = (*(int*)x_sockFD);
  int range[2];

  int nbytes=recv(sockFD, &range, 2*sizeof(int), 0);
  if (nbytes <= 0) {
    fprintf(logfile,"recieving input failed - Random Number\n");
    fflush(logfile);
    return 0;
  }

  int rand_value = rand()%((range[1] - range[0]) + 1) + range[0];

  pthread_mutex_lock(&mutex);

  char result[128];
  int index = 0;
  sprintf(&result[index], "Random Number %d ", rand_value);
  fprintf(logfile, "Random Number for range %d and %d : %s\n", range[0], range[1], result);
  fflush(logfile);
  if (send(sockFD, &rand_value, 1*sizeof(int) , 0) < 0) {
    fprintf(logfile,"Send failed - Random Number\n");
    fflush(logfile);
    return 0;
  }
  fprintf(logfile,"Send Successful - Random Number\n");
  fflush(logfile);

  pthread_mutex_unlock(&mutex);

  return 0;
}

void* fibonacci(void* x_sockFD){
   int sockFD = (*(int*)x_sockFD);
   int x;

   int nbytes = recv(sockFD,&x,sizeof(int),0);
   if(nbytes<=0){
     fprintf(logfile,"Recieve of fib no. failed\n");
     fflush(logfile);
     return 0;
   }
   int fib[x+1];
   fib[0] = 0;
   fib[1] = 1;

   for (int i = 2;i <= x; i++) {
    fib[i] = fib[i-1]+fib[i-2];
  }

   pthread_mutex_lock(&mutex);
   char result[128];
   int index = 0;
   sprintf(&result[index], "Fibonacci : %d ", fib[x]);
   fprintf(logfile, "Fibonacci for %d : %s\n", x, result);
   fflush(logfile);
   if (send(sockFD , &fib[x], 1*sizeof(int) , 0) < 0) {
     fprintf(logfile,"Send failed - Fibonacci\n");
     fflush(logfile);
     return 0;
   }
   fprintf(logfile,"Send Successful - Fibonacci\n");
   fflush(logfile);

   pthread_mutex_unlock(&mutex);

   return 0;
}


void* sorting(void* x_sockFD){

  int sockFD = (*(int*)x_sockFD);

  pthread_mutex_lock(&mutex);

  int sort_size = 0;
  int nbytes = recv(sockFD, &sort_size, 1*sizeof(int), 0);
  if (nbytes <= 0) {
    fprintf(logfile,"Recieve of fsort size failed\n");
    fflush(logfile);
    return 0;
  }

  int list[sort_size];
  nbytes = recv(sockFD, &list, sort_size*sizeof(int), 0);
  if ( nbytes <= 0 ) {
    fprintf(logfile,"Recieve of list failed\n");
    fflush(logfile);
    return 0;
  }

  for (int i = 0;i < sort_size; i++) {
    for (int j = 0; j < sort_size; j++) {
      if (list[i] < list[j]) {
        int temp = list[j];
        list[j] = list[i];
        list[i] = temp;
      }
    }
  }

  char result[sort_size];
  int index = 0;
  index += sprintf(&result[index], "Sorted List : ");
  for (int i=0; i<sort_size; i++) {
    index += sprintf(&result[index], "%d ", list[i]);
  }
  fprintf(logfile,"%s\n",result);
  fflush(logfile);
  if (send(sockFD , &list, sort_size*sizeof(int) , 0) < 0) {
    fprintf(logfile,"Send failed\n");
    fflush(logfile);
    return 0;
  }
  pthread_mutex_unlock(&mutex);

  return 0;
}

char* address(int sockFD){
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  getsockname (sockFD, (struct sockaddr *) &addr, &len);
  char* ip = (char*) malloc (128*sizeof(char));
  ip = inet_ntoa(addr.sin_addr);
  int port = ntohs(addr.sin_port);
  sprintf(ip, "%s %d", ip, port);
  return ip;
}
