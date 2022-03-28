#ifndef GAME__NODE_H_
#define GAME__NODE_H_

template<typename T>

class Node {
 public:
  T data;
  Node<T> *next;

  Node();
  Node(T data);
  void InsAfter(Node<T> *&q);
  void InsBefore(Node<T> *&q);
};

#endif //GAME__NODE_H_
