#include <iostream>
#include <thread>
#include <ctime>
#include "SynchronizedList.h"

bool isReady = false;
SynchronizedList _sync_list;

void producer()
{
	for (;;)
	{
		if (_sync_list.Push()) break;
	}
	isReady = true;
	printf("Producer thread finish...\n");
}

void consumer()
{	
	while (!isReady)
	{
		//std::cout << "wait for end.." << std::endl;
	}
	while (!_sync_list.IsEmpty())
	{
		_sync_list.Hash();
	}	
}

int main()
{
	std::srand(std::time(NULL));
	std::thread th1(producer);
	std::thread th2(producer);
    std::thread th3(consumer);
	std::thread th4(consumer);
	th1.join();
	th2.join();
	th3.join();
	th4.join();

	_sync_list.WriteToFile();
	_sync_list.ClearMemory();

	return 0;
}