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

#ifndef VOXELMAN_QUEUE_H
#define VOXELMAN_QUEUE_H

#include "core/reference.h"

#include "core/os/copymem.h"
#include "core/os/memory.h"
#include "core/os/rw_lock.h"
#include "core/variant.h"

template <class T>
class VMQueue {

public:
	void enqueue(T obj);
	T dequeue();
	T peek();

	void resize(int new_size);

	VMQueue(int initial_size);
	VMQueue();
	~VMQueue();

protected:
	void _resize(int new_size);

private:
	RWLock *_lock;

	int _head;
	int _count;
	int _size;
	T *_items;
};

class VoxelmanQueue : public Reference {
	GDCLASS(VoxelmanQueue, Reference);

public:
	void enqueue(Variant obj);
	Variant dequeue();
	Variant peek();

	void resize(int new_size);

	VoxelmanQueue();
	~VoxelmanQueue();

protected:
	static void _bind_methods();

private:
	VMQueue<Variant> _queue;
};

#endif