/*
 * Simple queue with maximum size.
 */

#ifndef queue_h
#define queue_h

#include <Arduino.h>

void die(const char *message)
{
  Serial.println(message);
}

class Queue {
  public:
    Queue(const int s);
    ~Queue();
    void push(const float i);
    float pop();
    bool isEmpty() const;
    bool isFull() const;

  private:
    float *contents;
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
  contents = (float *) malloc (sizeof (float) * size);
  if (contents == NULL) die("out of memory");
}

Queue::~Queue() {
  free(contents);
  contents = NULL;
  size = 0;
  items = 0;
  head = 0;
  tail = 0;
}

void Queue::push(const float i) {
  if (isFull()) die("bad push");
  contents[tail++] = i;
  if (tail == size) tail = 0;
  items++;
}
float Queue::pop() {
  if (isEmpty()) die("bad pop");
  float item = contents[head++];
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
