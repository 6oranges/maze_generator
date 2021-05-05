#ifndef _STACK_H_
#define _STACK_H_

template <class T>
class Node {
public:
  Node(T data);

  T getData() const;
  Node<T>* getNext() const;
  void setNext( Node<T>* item );
  
private:
  T mData;
  Node<T> *mNext;
  
};
template <class T>
Node<T>::Node(T data)
  : mData(data),mNext(0) {
}
template <class T>
T Node<T>::getData() const{
    return mData;
}
template <class T>
Node<T>* Node<T>::getNext() const{
    return mNext;
}
template <class T>
void Node<T>::setNext( Node<T>* node ){
    mNext=node;
}

template <class T>
class Stack {
public:
  Stack();
  ~Stack();

  T pop();
  T top() const;
  void push( const T item );
  bool isEmpty() const;
  
private:
  Node<T> *mTop;
  
};

template <class T>
Stack<T>::Stack()
  : mTop(0) {
}
template <class T>
Stack<T>::~Stack(){
    while (mTop!=0){
        pop();
    }
}
template <class T>
T Stack<T>::pop(){
    if (mTop!=0){
        T item=mTop->getData();
        Node<T>* next = mTop->getNext();
        delete mTop;
        mTop=next;
        return item;
    }
    else{
        throw "Stack is empty";
    }
}
template <class T>
T Stack<T>::top() const{
    if (mTop!=0){
        return mTop->getData();
    }
    else{
        throw "Stack is empty";
    }
}
template <class T>
void Stack<T>::push(T item){
    Node<T>* next= new Node<T>(item);
    next->setNext(mTop);
    mTop=next;
}
template <class T>
bool Stack<T>::isEmpty() const{
    return mTop==0;
}
#endif
