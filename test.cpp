// g++ -std=c++11 -o test test.cpp -I./ -lpthread

#include <rwlock.h>
#include <thread>
#include <iostream>
#include <string>
#include <random>
#include <ctime>


using namespace std;
using std::string;


RWLock rwlck(WritersPriority);
int gCounter = 0;


int randTime_ms()
{
    std::default_random_engine random(time(NULL));
    std::uniform_int_distribution<int> dis1(1, 5);
	
	return dis1(random);
}


void readFunc(string name)
{
	rwlck.AcquireShared();
	
	std::this_thread::sleep_for(std::chrono::milliseconds(randTime_ms()*100));
	std::cout << name << ", Counter:" << gCounter << std::endl;
	
	
	rwlck.ReleaseShared();
}

void writeFunc(string name)
{
	rwlck.AcquireExclusive();
	std::this_thread::sleep_for(std::chrono::milliseconds(randTime_ms()*1000));
	++gCounter;
	std::cout << name << ", Counter:" << gCounter << std::endl;
	
	rwlck.ReleaseExclusive();
}

void createReader()
{
	for(int i = 0; i < 30; ++i)
	{
		string content = string("reader ") + string(to_string(i));
		new std::thread(readFunc, content);
		std::this_thread::sleep_for(std::chrono::milliseconds(randTime_ms()*1000));
	}
}

void createWriter()
{
	for(int i = 0; i < 6; ++i)
	{
		string content = string("writer ") + string(to_string(i));
		new std::thread(writeFunc, content);
		std::this_thread::sleep_for(std::chrono::milliseconds(randTime_ms()*4000));
	}
}

int main()
{

	new std::thread(createReader);
	new std::thread(createWriter);

	
	std::this_thread::sleep_for(std::chrono::milliseconds(30000));
	
	return 0;
}
