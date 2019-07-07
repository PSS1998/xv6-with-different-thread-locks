#include "types.h"
#include "fcntl.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "readers_writerlock.h"


void init_ReadersWriterlock(struct RWLock *l){
  lock_t_init(&(l->writerlock));
  lock_t_init(&(l->rlock));
  l->readers = 0;
  l->writer = 0;
  l->writeRequest = 0;
}

void LockForRead(struct RWLock *l) {
  getlock_t(&(l->rlock));
  l->readers++;
  if(l->readers == 1){
    getlock_t(&(l->writerlock));
  }
  unlock_t(&(l->rlock));
}

void LockForWrite(struct RWLock *l) {
  getlock_t(&(l->writerlock));
}

void UnlockForRead(struct RWLock *l) {
  getlock_t(&(l->rlock));
  l->readers--;
  if(l->readers == 0){
    unlock_t(&(l->writerlock));
  }
  unlock_t(&(l->rlock));
}

void UnlockForWrite(struct RWLock *l) {
  unlock_t(&(l->writerlock));
}
