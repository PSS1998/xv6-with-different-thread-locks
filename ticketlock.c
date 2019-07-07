#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
#include "ticketlock.h"



void lock_t_init(struct __lock_t *m) {
  m->flag = 0;
  m->now_serving = 0;
}

void getlock_t(struct __lock_t *m) {
  int my_ticket;
  my_ticket = fetch_and_add(&m->now_serving, 1);
  if (m->flag == 0) {
    m->flag = 1; // lock is acquired
  } else {
    sleep_t((void*)my_ticket);
  }
}

void unlock_t(struct __lock_t *m) {
  m->now_serving++;
  int lock_needed = wakeup_t((void*)m->now_serving);
  if(!lock_needed)
    m->flag = 0;
}
