#include "ticketlock.h"


struct RWLock {
	int readers;
	int writer;
	int writeRequest;
  struct __lock_t rlock;
  struct __lock_t writerlock;
};


void LockForRead(struct RWLock *l);
void LockForWrite(struct RWLock *l);
void UnlockForRead(struct RWLock *l);
void UnlockForWrite(struct RWLock *l);
void init_ReadersWriterlock(struct RWLock *l);
