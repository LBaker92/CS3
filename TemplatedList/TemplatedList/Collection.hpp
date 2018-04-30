#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include "list.hpp"

template <typename T>
class Collection;

template <typename T>
bool Equal(const Collection<T> & lhs, const Collection<T> & rhs);

template <typename T>
class Collection {
public:
    Collection() : head_(nullptr), tail_(nullptr), size_(0) {}  //Default Constructor
    Collection(const Collection<T> & other);                    //Copy Constructor
    Collection(Collection<T> && other);                         //Move Constructor
    ~Collection();                                              //Destructor

    Collection<T> & operator= (const Collection<T> & other);    //Assignment Overload
    Collection<T> & operator= (const Collection<T> && other);   //Move Assignment Overload

    void AddItem(const T & item);
    void RemoveItem(const T & item);
    void PrintCollection();
    T LastItem() const { return tail_->get_data(); }

    friend bool Equal<T>(const Collection<T> & lhs, const Collection<T> & rhs);

private:
    Node<T> * head_, * tail_;
    int size_;
};
#endif

template <typename T>
Collection<T>::Collection(const Collection<T> & other) : head_(nullptr), tail_(nullptr), size_(0) {
    size_ = other.size_;   
    for (Node<T> * i = other.head_; i != nullptr; i = i->get_next()) {  //Iterate through other's list
        Node<T> * new_node = new Node<T>(*i);   //Create a new node with the iterator's data
        if (head_ == nullptr) { //If the lhs does not have any nodes in the list
            head_ = new_node;
            tail_ = new_node;
        }
        else {  //If there's already an item on the lhs's list
            tail_->set_next(new_node);
            tail_ = new_node;
        }
    }
}

template<typename T>
Collection<T>::Collection(Collection<T>&& other) : head_(nullptr), tail_(nullptr), size_(0) {
    size_ = other.size_;
    head_ = other.head_;
    tail_ = other.tail_;
    other.size_ = 0;
    other.head_ = nullptr;
    other.tail_ = nullptr;
}

template <typename T>
Collection<T>::~Collection() {

    for (Node<T> * i = head_; i != nullptr; i = head_) {   //Iterate through the list
        head_ = head_->get_next();   //Move the head to the next item on the list
        delete i;    //Delete the previous head node
        --size_;
    }
}

template<typename T>
Collection<T> & Collection<T>::operator=(const Collection<T> & other)
{
    if (this == &other) {  //If the lhs is the same memory location as other, ie: they are the same thing.
        return *this;
    }
    if (other.size_ == 0) {
        return *this;
    }
    if (this != &other) {  //If the lhs is not the same as other
        delete head_;
        size_ = other.size_;
        for (Node<T> * i = other.head_; i != nullptr; i = i->get_next()) {  //Iterate through other's list
            Node<T> * new_node = new Node<T>(*i);   //Create a new node with the iterator's data
            if (head_ == nullptr) { //If the lhs does not have any nodes in the list
                head_ = new_node;
                tail_ = new_node;
            }
            else {  //If there's already an item on the lhs's list
                tail_->set_next(new_node);
                tail_ = new_node;
            }
        }
    }
    return *this;
}

template <typename T>
Collection<T> & Collection<T>::operator=(const Collection<T> && other) {
    if (this != &other) {
        delete head_;
        if (this == &other) {  //If the lhs is the same memory location as other, ie: they are the same thing.
            return *this;
        }
        if (other.size_ == 0) {
            return *this;
        }
        if (this != &other) {  //If the lhs is not the same as other
            delete head_;
            size_ = other.size_;
            head_ = other.head_;
            tail_ = other.tail_;
            other.size_ = 0;
            other.head_ = nullptr;
            other.tail_ = nullptr;
        }
        return *this;
    }
}

template <typename T>
void Collection<T>::AddItem(const T & item) {
    Node<T> * temp = new Node<T>;   //Make a new node
    temp->set_data(item);    //New node's data becomes the item
    if (head_ == nullptr) { //If there's nothing on the list
        head_ = temp;
        tail_ = temp;
    }
    else {  //If there is an item on the list
        tail_->set_next(temp);
        tail_ = temp;
    }
    ++size_;
}

template <typename T>
void Collection<T>::RemoveItem(const T & item) {
    if (size_ == 0) {   //If there's nothing on the list to remove
        return;
    }
    Node<T> * i = head_, * prev = nullptr, * node_to_delete = nullptr;
    while (i != nullptr) {   //While the iterator is not at the end of the list
        if (i == head_ && head_->get_data() == item) {  //If the iterator is at the beginning of the list and the Node needs removed
            node_to_delete = i;   
            i = i->get_next();
            delete node_to_delete;
            --size_;
        }
        else if (i->get_data() == item) { //If the iterator is the item to delete
            node_to_delete = i;
            prev->set_next(i->get_next()); //The previous node skips to the node after the node to delete
            if (prev->get_next() == nullptr) {
                tail_ = prev;
            }
            i = i->get_next();
            delete node_to_delete;
            --size_;
        }
        else {  //If current node is does not contain the item
            prev = i;
            i = i->get_next();
        }
    }
}

template <typename T>
void Collection<T>::PrintCollection() {
    for (Node<T> * i = head_; i != nullptr; i = i->get_next()) { //Iterate through the list
        std::cout << "[" << i->get_data() << "]->";
    }
    std::cout << "[NULL]" << std::endl;
}

template <typename T>
bool Equal(const Collection<T> & lhs, const Collection<T> & rhs) {
    if (lhs.size_ != rhs.size_) { //If the two collections don't have the same size
        return false;
    }
    Node<T> * j = rhs.head_;
    for (Node<T> * i = lhs.head_; i != nullptr; i = i->get_next()) {    //Iterate through the first collection and compare the node to the second collection's node
        if (i->get_data() != j->get_data() && i->get_next() != j->get_next()) { //If contents of the node are not the same
            return false;
        }
        j = j->get_next();
    }
    return true;
}