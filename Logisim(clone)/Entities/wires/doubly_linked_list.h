#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

template<typename Container, typename Type, typename T>
class DLLNextIterator {
public:
    DLLNextIterator() = delete;
    
    DLLNextIterator(Type* head) : it{head} {}
    
    bool operator== (const DLLNextIterator& other) {
        return it == other.it;
    }
    
    bool operator!= (const DLLNextIterator& other) {
        return it != other.it;
    }
    
    T& operator* () {
        return it->value;
    }
    
    DLLNextIterator const& operator++ () {
        it = it->next;
        return *this;
    }
    
private:
    Type* it;
};

template <typename T>
struct DLLNode {
    T value;
    DLLNode *next;
    DLLNode *prev;
    
    DLLNode<T>() {}
    DLLNode<T>(T value) : value{value}, next{nullptr}, prev{nullptr} {}
};

template <typename T>
class DoublyLinkedList {
public:
    DLLNode<T> *head;
    DLLNode<T> *tail;
    
    DoublyLinkedList() : head{nullptr}, tail{nullptr}, m_size{0} {}
    
    void push_back(T value) {
        DLLNode<T>* newNode = new DLLNode<T>(value);
        
        if(head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        
        m_size++;
    }
    
    T& front() const {
        return head->value;
    }
    
    T& back() const {
        return tail->value;
    }
    
    int size() const {
        return m_size;
    }
private:
    int m_size;
};

template <typename T>
inline DLLNextIterator<DoublyLinkedList<T>, DLLNode<T>, T> begin(DoublyLinkedList<T>& container) {
    return  DLLNextIterator<DoublyLinkedList<T>, DLLNode<T>, T>(container.head);
}

template <typename T>
inline DLLNextIterator<DoublyLinkedList<T>, DLLNode<T>, T> end(DoublyLinkedList<T>& container) {
    return  DLLNextIterator<DoublyLinkedList<T>, DLLNode<T>, T>(nullptr);
}

template <typename T>
inline DLLNextIterator<DoublyLinkedList<T> const, DLLNode<T>, T> begin(DoublyLinkedList<T> const& container) {
    return  DLLNextIterator<DoublyLinkedList<T> const, DLLNode<T>, T>(container.head);
}

template <typename T>
inline DLLNextIterator<DoublyLinkedList<T> const, DLLNode<T>, T> end(DoublyLinkedList<T> const& container) {
    return  DLLNextIterator<DoublyLinkedList<T> const, DLLNode<T>, T>(nullptr);
}

#endif
