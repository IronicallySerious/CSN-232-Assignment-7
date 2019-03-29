#include <iostream>  
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex Mutex;
std::condition_variable Cv;

int Meal = 0;

void waiter(int ordernumber)
{
	std::unique_lock<std::mutex> lock(Mutex);

	while (Meal == 0)
	{
		Cv.wait(lock);
	}

	std::cout << "Order: " << ordernumber + 1 << " being prepared with ";
	std::cout << Meal - 1 << " meals also ready." << std::endl;
	Meal--;
}

void makeMeal(int ordernumber)
{
	std::unique_lock<std::mutex> lock(Mutex);
	Meal++;
	Cv.notify_one();
}

int main()
{
	std::thread producers[10];
	std::thread consumers[10];

	for (int order = 0; order < 10; order++)
	{
		producers[order] = std::thread(makeMeal, order);
		consumers[order] = std::thread(waiter, order);
	}

	for (int order = 0; order < 10; order++)
	{
		consumers[order].join();
		producers[order].join();
	}

	std::cin.get();

	return 0;
}