#include <iostream>
#include <thread>
#include <mutex>

constexpr unsigned int BUFFER_SIZE = 100;
int itemNumber = 0;
int maxRun = 20;

std::mutex mutex;

int turn = 0;
int mut = 1;

void semWait(std::string status)
{
	if (status == "empty")
	{
		while (itemNumber == BUFFER_SIZE);
		mut--;
	}
	else
	{
		while (itemNumber == 0);
		mut--;
	}
}

void semSignal(std::string status)
{
	if (status == "empty")
	{
		mut++;
	}
	else
	{
		mut++;
	}
}

void producer() 
{
	while (maxRun > 0) 
	{
		semWait("empty");
		mutex.lock();
		itemNumber += 1;

		std::cout << "Produced buffer item: " << itemNumber << "\n";
		
		maxRun -= 1;
		mutex.unlock();
		semSignal("full");
	}
}

void consumer() 
{
	while (maxRun > 0) 
	{
		semWait("full");
		mutex.lock();
		
		std::cout << "Consumed buffer item: " << itemNumber << "\n";
		
		itemNumber -= 1;
		maxRun -= 1;
		mutex.unlock();
		semSignal("empty");
	}
}

int main()
{
	std::thread t1(producer);
	std::thread t2(consumer);

	t1.join();
	t2.join();
	
	return 0;
}
