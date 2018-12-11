#ifndef _LL_STRUCTS_H_
#define _LL_STRUCTS_H_

struct node {
    int data;
    struct node* next;
};

struct list {
    struct node* head;
};

#endif
