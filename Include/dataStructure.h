#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

int dequeue();
void enqueue(const int value);
struct ListNode {
  int value;
  struct ListNode *next; 
};
typedef struct ListNode ListNode;

struct Queue {
  ListNode *head;
  ListNode *tail;
};
typedef struct Queue Queue;

#endif // !DATASTRUCTURE_H
