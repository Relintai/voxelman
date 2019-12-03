#include "voxelman_unbounded_queue.h"

/// VMQUeue

template<typename T>
void VMUBQueue<T>::enqueue(T obj) {
    _enqueue_lock->write_lock();

    UBQueueNode<T> *n = memnew(UBQueueNode<T>(obj));

    _tail->next = n;
    _tail = n;

    _enqueue_lock->write_unlock();
}

template<typename T>
T VMUBQueue<T>::dequeue() {
    T result;

    _dequeue_lock->write_lock();

    if (_head->next == NULL) {
        return result;
    }

    result = _head->next->data;
    
    UBQueueNode<T> *h = _head;

    _head = _head->next;

    memdelete(h);

    _dequeue_lock->write_unlock();

    return result;
}

template<typename T>
T VMUBQueue<T>::peek() {
    return _head->data;
}

template<typename T>
VMUBQueue<T>::VMUBQueue() {
    _head = memnew(UBQueueNode<T>);
    _tail = _head;

    _enqueue_lock = RWLock::create();
    _dequeue_lock = RWLock::create();
}

template<typename T>
VMUBQueue<T>::~VMUBQueue() {
    memdelete(_enqueue_lock);
    memdelete(_dequeue_lock);

    UBQueueNode<T> *h = _head;

    while (h) {
        UBQueueNode<T> *n = h->next;

        memdelete(h);

        h = n;
    }
}

///VoxelmanUnboundedQueue

void VoxelmanUnboundedQueue::enqueue(Variant obj) {
    _queue.enqueue(obj);
}
Variant VoxelmanUnboundedQueue::dequeue() {
    return _queue.dequeue();
}
Variant VoxelmanUnboundedQueue::peek() {
    return _queue.peek();
}

VoxelmanUnboundedQueue::VoxelmanUnboundedQueue() {
}

VoxelmanUnboundedQueue::~VoxelmanUnboundedQueue() {
}

void VoxelmanUnboundedQueue::_bind_methods() {
    ClassDB::bind_method(D_METHOD("enqueue", "obj"), &VoxelmanUnboundedQueue::enqueue);
    ClassDB::bind_method(D_METHOD("dequeue"), &VoxelmanUnboundedQueue::dequeue);
    ClassDB::bind_method(D_METHOD("peek"), &VoxelmanUnboundedQueue::peek);
}
