#include <algorithm>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

// Do not compile with SINGLE & MULTI defined
#define MULTI

// Initial string to permute
const std::string key = "0123456789AB";
// The final solution for the test case
const std::string solution = "A0124B395876";
// The amount of threads that will be created
constexpr unsigned long long thread_count = 4;

// Global thread variables
std::condition_variable condition;
std::mutex m;

// Factorial
unsigned long long fact(unsigned long long x)
{
	for (unsigned long long i = x - 1; i > 0; i--)
		x *= i;
	return x;
}

std::string permute(unsigned long long k, std::string s)
{
	for (unsigned long long j = 1; j < s.length(); j++)
	{
		std::swap(s[static_cast<std::size_t>(k % (j + 1))], s[static_cast<std::size_t>(j)]);
		k = k / (j + 1);
	}

	return s;
}

void thread_begin(unsigned long long start, unsigned long long end, std::string key, unsigned long long id)
{
	for (; start < end; start++)
	{
		if (permute(start, key) == solution)
		{
			std::cout << "Thread #" << id << " has found the solution on count " << start << '\n';
			condition.notify_all();
			break;
		}
	}
}

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

#ifdef SINGLE

	// Single Thread
	for (int i = 0; i < fact(key.length()); i++)
	{
		if (permute(i, key) == solution)
		{
			std::cout << "Solution found on iteration #" << i << "\n";
			break;
		}
	}
	
	auto stop = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	
	std::cout << "Elapsed time: " << dur.count() << "ms\n";

#endif

#ifdef MULTI

	// Multi Thread
	std::vector<std::thread> threads;
	const unsigned long long scalar = fact(key.length()) / thread_count;

	std::cout << "Calculated scalar " << scalar 
		<< ", with key length " << key.length() << " and thread count " << thread_count << '\n';

	for (unsigned long long t = 0; t < thread_count; t++)
	{
		threads.push_back(std::thread{ thread_begin, t * scalar, (t + 1) * scalar, key, t });
		std::cout << "Thread #" << t << " created\t- [" << t * scalar << ", " << (t + 1) * scalar << ")\n";
	}

	std::unique_lock<std::mutex> lock{ m };
	condition.wait(lock);

	auto stop = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << "Elapsed time: " << dur.count() << "ms\n";

	for (auto& t : threads)
		t.join();

	stop = std::chrono::high_resolution_clock::now();
	dur = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	std::cout << "Total time: " << dur.count() << "ms\n";

#endif

	return 0;
}