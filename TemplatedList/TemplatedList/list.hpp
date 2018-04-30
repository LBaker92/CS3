// Node class tempate used to contruct lists
// Mikhail Nesterenko
// 5/01/00

#ifndef LIST_H_
#define LIST_H_

// the pointer part of every list is the same
// lists differ by data part
template <typename T>
class Node {
public:
    Node() : next_(nullptr) {}

    //Accessors
    T get_data()const { return data_; }
    Node<T>* get_next() const { return next_; }
    
    //Mutators
    void set_data(const T& data) { data_ = data; }
    void set_next(Node<T> * const next) { next_ = next; }

private:
    T data_;
    Node<T> *next_;
};
#endif // LIST_H_