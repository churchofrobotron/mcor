/*
 * Simple queue with maximum size.
 */

#ifndef queue_h
#define queue_h

#include <Arduino.h>

class Queue {
  public:
    Queue(const int s);
    ~Queue();
    void push(const int i);
    int pop();
    bool isEmpty() const;
    bool isFull() const;

  private:
    int *contents;
    int size;
    int items;
    int head;
    int tail;
};

Queue::Queue(const int s = 10) {
  size = s;
  items = 0;
  head = 0;
  tail = 0;
  contents = (int *) malloc (sizeof (int) * size);
  //if (contents == NULL) die
}

Queue::~Queue() {
  free(contents);
  contents = NULL;
  size = 0;
  items = 0;
  head = 0;
  tail = 0;
}

void Queue::push(const int i) {
  //if (isFull()) die
  contents[tail++] = i;
  if (tail == size) tail = 0;
  items++;
}
int Queue::pop() {
  //if (isEmpty()) die
  int item = contents[head++];
  items--;
  if (head == size) head = 0;
  return item;
}

bool Queue::isEmpty() const {
  return items == 0;
}

bool Queue::isFull() const {
  return items == size;
}

#endif 
