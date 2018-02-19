#pragma once
#include "lock.h"

template<class Key, class Value>
class Nextlist {
private:
	struct _L {
		struct _L* next = nullptr;
		Key key;
		Value value;
	};
	_L* _data = nullptr;
	LSECT _lock;
	int _size = 0;
public:
	Nextlist<Key, Value>()
	{
		_lock = lock_new();
	}

	void lock() {
		lock_lock(_lock);
	}

	void unlock() {
		lock_unlock(_lock);
	}

	void push(Key key, Value value)
	{
		lock();
		_L *el = new _L();
		el->next = _data;
		el->key = key;
		el->value = value;
		_data = el;
		_size++;
		unlock();
	}

	void pop(Key key)
	{
		lock();
		_L *last = _data;
		for (_L *el = _data; el != nullptr; el = el->next) {
			if (key == el->key) {
				last->next = el->next;
				delete el;
				_size--;
				break;
			}
			last = el;
		}
		unlock();
	}

	bool exists(Key key)
	{
		lock();
		for (_L *el = _data; el != nullptr; el = el->next) {
			if (key == el->key) {
				unlock();
				return true;
			}
		}
		unlock();
		return false;
	}

	Value& operator [] (Key key)
	{
		lock();
		for (_L *el = _data; el != nullptr; el = el->next) {
			if (key == el->key) {
				unlock();
				return el->value;
			}
		}

		unlock();
		throw 0;
	}

	int size() {
		return _size;
	}

	_L* data()
	{
		return _data;
	}
};
