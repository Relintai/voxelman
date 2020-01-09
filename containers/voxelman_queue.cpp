#include "voxelman_queue.h"

/// VMQUeue

template <typename T>
void VMQueue<T>::enqueue(T obj) {
	_lock->write_lock();

	if (_size == _count) {
		_resize(_size + 10);
	}

	_items[(_head + _count) % _size] = obj;
	++_count;

	_lock->write_unlock();
}

template <typename T>
T VMQueue<T>::dequeue() {
	if (_size == 0)
		return T();

	if (_count == 0)
		return T();

	T obj;

	_lock->write_lock();

	obj = _items[_head];

	_items[_head] = T();

	++_head;
	--_count;

	if (_head >= _size)
		_head -= _size;

	_lock->write_unlock();

	return obj;
}

template <typename T>
T VMQueue<T>::peek() {
	T obj;

	_lock->read_lock();
	obj = _items[_head];
	_lock->read_unlock();

	return obj;
}

template <typename T>
void VMQueue<T>::resize(int new_size) {
	_lock->write_lock();

	_resize(new_size);

	_lock->write_unlock();
}

template <typename T>
void VMQueue<T>::_resize(int new_size) {
	if (new_size == _size)
		return;

	if (new_size > _size) {

		T *narr = memnew_arr(T, new_size);
		//copymem(narr, _items, sizeof(T) * _size);

		for (int i = 0; i < _size; ++i) {
			narr[i] = _items[i];
		}

		memdelete_arr(_items);
		_items = narr;

		_size = new_size;

		return;
	}
}

template <typename T>
VMQueue<T>::VMQueue(int initial_size) {
	_items = NULL;
	_head = 0;
	_count = 0;
	_size = initial_size;

	if (unlikely(initial_size <= 0)) {
		_size = 10;

		print_error("initial_size <= 0");
	}

	_lock = RWLock::create();

	_items = memnew_arr(T, _size);
}

template <typename T>
VMQueue<T>::VMQueue() {
	_items = NULL;
	_head = 0;
	_count = 0;
	_size = 10;

	_lock = RWLock::create();

	_items = memnew_arr(T, _size);
}

template <typename T>
VMQueue<T>::~VMQueue() {
	if (_items)
		memdelete_arr(_items);

	memdelete(_lock);
}

///VoxelmanQueue

void VoxelmanQueue::enqueue(Variant obj) {
	_queue.enqueue(obj);
}
Variant VoxelmanQueue::dequeue() {
	return _queue.dequeue();
}
Variant VoxelmanQueue::peek() {
	return _queue.peek();
}

void VoxelmanQueue::resize(int new_size) {
	_queue.resize(new_size);
}

VoxelmanQueue::VoxelmanQueue() {
}

VoxelmanQueue::~VoxelmanQueue() {
}

void VoxelmanQueue::_bind_methods() {
	ClassDB::bind_method(D_METHOD("enqueue", "obj"), &VoxelmanQueue::enqueue);
	ClassDB::bind_method(D_METHOD("dequeue"), &VoxelmanQueue::dequeue);
	ClassDB::bind_method(D_METHOD("peek"), &VoxelmanQueue::peek);
	ClassDB::bind_method(D_METHOD("resize", "new_size"), &VoxelmanQueue::resize);
}
