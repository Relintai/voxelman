/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef VOXELMAN_UNBOUNDED_QUEUE_H
#define VOXELMAN_UNBOUNDED_QUEUE_H

#include "core/reference.h"

#include "core/os/copymem.h"
#include "core/os/memory.h"
#include "core/os/rw_lock.h"
#include "core/variant.h"

template <class T>
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

template <class T>
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