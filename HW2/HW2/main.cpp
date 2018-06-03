#include <cstdio>
#include <thread>
#include <list>
#include <atomic>
#include <mutex>
#include "tree.h"
#include <iostream>
#include <fstream>
#include <conio.h>

std::atomic<int> _counter = 0;
std::list<std::string*> _list;
MerkleTree _tree;
int _dwRootCount = 128;
std::mutex _mutex;
std::ofstream _outFile("log.txt");

char _char_list[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
std::string* GetRandomString()
{
	std::string* random_string = new std::string();
	*random_string = "";
	for (int i = 0; i < 256; i++) 
	{
		*random_string += _char_list[rand() % sizeof _char_list];
	}
	return random_string;
}
void producer()
{
	while (_counter < _dwRootCount) 
	{
		std::lock_guard<std::mutex> locker(_mutex);
		std::string* str = GetRandomString();
		_list.push_back(str);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void consumer()
{
	while (_counter < _dwRootCount) 
	{
		std::lock_guard<std::mutex> locker(_mutex);
		if (_list.size() >= 4) 
		{
			std::string* random_strings = new std::string[4];
			for (int i = 0; i < 4; i++) 
			{
				std::string* str = _list.back();				
				random_strings[i] = *str;
				delete str;
				_list.pop_back();
			}
			_tree.BuildTree(random_strings);
			_outFile << *_tree.GetRootHash() << std::endl;
			_tree.ClearMemory();
			//std::cout << tr.root << std::endl;
			_counter++;
		}
	}
}

int main()
{
	srand(time(NULL));
	std::thread th1(producer);
	std::thread th2(consumer);
	th1.join();
	th2.join();
	printf("MerkleTree wrote!\n");
	_getch();
	return 0;
}