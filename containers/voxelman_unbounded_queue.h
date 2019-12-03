#ifndef VOXELMAN_UNBOUNDED_QUEUE_H
#define VOXELMAN_UNBOUNDED_QUEUE_H

#include "core/reference.h"

#include "core/os/copymem.h"
#include "core/os/memory.h"
#include "core/os/rw_lock.h"
#include "core/variant.h"

template<class T>
struct UBQueueNode {
    T data;
    UBQueueNode *next;

    UBQueueNode() {
        next = NULL;
    }

    UBQueueNode(T value) {
        data = value;
        next = NULL;
    }
};

template<class T>
class VMUBQueue {

public:
    void enqueue(T obj);
    T dequeue();
    T peek();

    VMUBQueue();
    ~VMUBQueue();

private:
    RWLock *_enqueue_lock;
    RWLock *_dequeue_lock;

    UBQueueNode<T> *_head;
    UBQueueNode<T> *_tail;
};

class VoxelmanUnboundedQueue : public Reference {
    GDCLASS(VoxelmanUnboundedQueue, Reference);

public:
    void enqueue(Variant obj);
    Variant dequeue();
    Variant peek();

    VoxelmanUnboundedQueue();
    ~VoxelmanUnboundedQueue();

protected:
    static void _bind_methods();

private:
    VMUBQueue<Variant> _queue;
};

#endif