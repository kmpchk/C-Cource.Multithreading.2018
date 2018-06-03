#include "SynchronizedList.h"
#include "picosha2.h"

#include <iostream>
#include <random>
#include <fstream>

SynchronizedList::SynchronizedList()
{
}

SynchronizedList::~SynchronizedList()
{
}

bool SynchronizedList::IsEmpty()
{
	return (_counter == 0);
}

bool SynchronizedList::Push()
{
	std::unique_lock<std::mutex> locker(_push_mutex);
	if (_list.size() == _dwBlockCount)
	{
		locker.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return true;
	}
	else
	{
		std::vector<unsigned char>* bytes = new std::vector<unsigned char>(_dwBlockCount);
		std::thread::id id = std::this_thread::get_id();
		int* ptr = (int*)&id;;
		for (int i = 0; i < _dwBlockCount; i++)
		{
			int num = (((std::rand()) ^ *ptr) % 255);
			bytes->push_back(num);
			num = 0;
		}
		CBlock* dataBlock = new CBlock();
		dataBlock->data = bytes;
		//std::cout << "Push Percent= " << (double)(_counter * 100) / _dwBlockCount << std::endl;		
		_list.push_back(dataBlock);
		_counter++;
		//printf("[ThreadId] %d", *ptr);
		printf("[Push] Percent: %.2f\n", (double)(_counter * 100) / _dwBlockCount);
		locker.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		return false;
	}
}

void SynchronizedList::Hash()
{		
	std::unique_lock<std::mutex> locker(_hash_mutex);
	_counter--;
	_counter2++;
	std::list<CBlock*>::iterator it = _list.begin();
	std::advance(it, _counter.load());
	CBlock* block = *it;
	std::string hash_hex_str = picosha2::hash256_hex_string(block->data);
	block->hash = hash_hex_str;
	printf("[Hash forming] Percent: %.2f\n", (double)(_counter2 * 100) / _dwBlockCount);
	//std::cout << std::this_thread::get_id();
	//printf(" Pos: %u, Hash: %s\n", (unsigned)_counter, block->hash.c_str());
	locker.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void SynchronizedList::ClearMemory()
{
	while (!_list.empty())
	{
		CBlock* block = _list.back();
		delete block->data;
		delete block;
		_list.pop_back();
	}
}

void SynchronizedList::WriteToFile()
{
	std::ofstream outFile("hashes.bin");
	for each (CBlock* block in _list)
	{
		outFile << block->hash << std::endl;
	}
}
