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