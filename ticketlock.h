#ifndef _TICKETLOCK_
#define _TICKETLOCK_


int
wakeup_t(void *chan);
void
sleep_t(void *chan);


struct __lock_t {
  int flag;
  int now_serving;
};

void lock_t_init(struct __lock_t *m);
void getlock_t(struct __lock_t *m);
void unlock_t(struct __lock_t *m);


#endif
