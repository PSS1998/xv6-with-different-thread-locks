#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ticketlock.h"
#include "readers_writerlock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



int ticketlockvariable;
struct __lock_t lk;

int
sys_ticketlockinit(void)
{
  lock_t_init(&lk);
  ticketlockvariable = 0;
  return ticketlockvariable;
}

int
sys_ticketlocktest(void)
{
  getlock_t(&lk);
  ticketlockvariable += 1;
  unlock_t(&lk);
  return ticketlockvariable;
}


//rw lock functions

uint int_to_int(uint k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

int findn(uint num)
{
  if ( num < 10 )
    return 1;
  if ( num < 100 )
    return 2;
  if ( num < 1000 )
    return 3;
  if ( num < 10000 )
    return 4;
  if ( num < 100000 )
    return 5;
  if ( num < 1000000 )
    return 6;
  if ( num < 10000000 )
    return 7;
  if ( num < 100000000 )
    return 8;
  if ( num < 1000000000 )
    return 9;
  return 10;
}

int rwturn(int temp_turn, int turn, uint temp_pattern){
  int i = turn-temp_turn;
  int j = 0;
  for(j=0; j<i-1; j++){
    temp_pattern /= 10;
  }
  turn = temp_pattern%10;
  return turn;
}

int rwlockvariable;
int rwlockturn;
struct RWLock rwlk;
struct __lock_t ticketlock;
uint pattern;

int
sys_rwinit(void)
{
  lock_t_init(&ticketlock);
  init_ReadersWriterlock(&rwlk);
  rwlockvariable = 0;
  rwlockturn = 0;
  //10010
  pattern = 18;
  return rwlockvariable;
}

int
sys_rwtest(void)
{
  getlock_t(&ticketlock);
  rwlockturn += 1;
  int temp_turn = rwlockturn;
  unlock_t(&ticketlock);

  uint temp_pattern = int_to_int(pattern);
  int turn = findn(temp_pattern);
  turn = rwturn(temp_turn, turn, temp_pattern);

  int temp = 0;
  if(turn == 1){
    LockForWrite(&rwlk);
    rwlockvariable += 1;
    temp = rwlockvariable;
    UnlockForWrite(&rwlk);
  }
  else if(turn == 0){
    LockForRead(&rwlk);
    temp = rwlockvariable;
    UnlockForRead(&rwlk);
  }

  return temp;
}
