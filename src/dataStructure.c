#include "../Include/dataStructure.h"
#include <stdlib.h>
#include <unistd.h>
ListNode *head = NULL;
ListNode *tail = NULL;

void enqueue(int value) {
  ListNode *node = malloc(sizeof(ListNode));
  node->value = value;
  node->next = NULL;
  if (!head || !tail) {
    head = node;
  } else {
    tail->next = node;
  }
  tail = node;
}

int dequeue() {
  if (!head || !tail) {
    return -1;
  } else {
    int value;
    ListNode *temp;
    value = head->value;
    temp = head;
    head = head->next;
    free(temp);
    return value;
  }
}
