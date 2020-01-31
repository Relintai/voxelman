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

#include "voxelman_unbounded_queue.h"

/// VMQUeue

template <typename T>
void VMUBQueue<T>::enqueue(T obj) {
	_enqueue_lock->write_lock();

	UBQueueNode<T> *n = memnew(UBQueueNode<T>(obj));

	_tail->next = n;
	_tail = n;

	_enqueue_lock->write_unlock();
}

template <typename T>
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

template <typename T>
T VMUBQueue<T>::peek() {
	return _head->data;
}

template <typename T>
VMUBQueue<T>::VMUBQueue() {
	_head = memnew(UBQueueNode<T>);
	_tail = _head;

	_enqueue_lock = RWLock::create();
	_dequeue_lock = RWLock::create();
}

template <typename T>
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
