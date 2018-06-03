#pragma once
#include <mutex>
#include <list>
#include <atomic>
#include "CBlock.h"

class SynchronizedList
{

private:
	std::mutex _push_mutex, _hash_mutex;
	std::list<CBlock*> _list;
	std::atomic<int> _counter = 0, _counter2 = 0;
	int _dwBlockCount = 512;
public:
	SynchronizedList();
	~SynchronizedList();
	bool Push();
	void Hash();
	bool IsEmpty();
	void ClearMemory();
	void WriteToFile();
};

