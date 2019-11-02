#ifndef _STACK_H_
#define _STACK_H_

#include "linklist.h"




extern struct list* stack_new();
extern struct listnode* stack_pop(struct list* stack);
extern struct listnode* stack_lookup(struct list* stack);
extern int stack_add(struct list* stack, char* val);

#endif // !_STACK_H_
