#include "types.h"
#include "user.h"

#define NCHILD 10

int main(){
  int pid;

  ticketlockinit();

  pid = fork();
  int i;
  for(i = 1; i < NCHILD; i++)
    if(pid > 0)
      pid = fork();

  if(pid < 0){
    printf(2, "fork error\n");
  }
  else if(pid == 0){
    printf(1, "child adding to shared counter\n");
    int var = ticketlocktest();
    printf(1, "%d\n", var);
  }
  else{
    for(i = 0; i < NCHILD; i++)
      wait();
    printf(1, "user program finished\n");
  }

  exit();
}
