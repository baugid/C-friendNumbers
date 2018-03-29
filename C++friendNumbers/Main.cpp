#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <algorithm>
#include <limits>
#include <iterator>
//pair of two unsigned long longs ordered after the first number
struct pair {
	unsigned long long val1;
	unsigned long long val2;
	pair(unsigned long long val1, unsigned long long val2) :val1(val1), val2(val2) {}
	bool operator<(pair other) {
		return (val1 < other.val1);
	}
};

std::ostream& operator<< (std::ostream& right, const pair& left) {
	return right << left.val1 << ", " << left.val2;
}

unsigned long long sumDivisors(const unsigned long long& n) {
	unsigned long long sum = 1;
	unsigned long long root = sqrtl(n);
	//add all divisors <sqrt(n) to sum
	for (unsigned long long i = 2; i < root; i++) {
		if (n%i == 0) {
			sum += i;
			sum += n / i;
		}
	}
	//check if sqrt(n) is natural and add it
	if (root*root == n) {
		sum += root;
	}
	return sum;
}

std::vector<pair> genFriendNumbsToMax(const unsigned long long& max) {
	std::vector<pair> out;
	std::mutex lock;
	unsigned int cores = std::thread::hardware_concurrency();
	std::thread* threads = new std::thread[cores]; // Create as much threads as needed
	for (unsigned int i = 0; i < cores; ++i) {
		threads[i] = std::thread([&out, &lock, min = i == 0 ? 2 : max / cores * i, threadMax = i == cores - 1 ? max : max / cores * (i + 1) - 1]{ // min is 1/15*i but zero and one should be left out the last thread must test the whole rest
			std::vector<pair> ownData;
			for (unsigned long long i = min; i <= threadMax; ++i) {
				unsigned long long dividerSum = sumDivisors(i);
				if (i < dividerSum&&i == sumDivisors(dividerSum)) {//pairs shouldn´t occur twice and check if it has a friend number
					//add the friend pair
					ownData.emplace_back(i, dividerSum);
				}
			}
			//append all to the total
			lock.lock();
			out.insert(out.cend(), ownData.begin(), ownData.end());
			lock.unlock();
			});
	}
	std::cout << "Setup done!\n";
	//join all threads
	for (unsigned int i = 0; i < cores; i++) {
		threads[i].join();
	}
	delete[] threads;
	return out;
}

int main() {
	unsigned long long max = 0;
	//fetch user input
inputLoop:
	std::cout << "Enter maximum value: ";
	if (!(std::cin >> max)) {
		std::cout << "Incorrect input. Please try again.\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		goto inputLoop;
	}
	if (max <= 2) {
		std::cout << "Maximum value must be greater than two!\n";
		goto inputLoop;
	}

	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();//start time measurement
	std::vector<pair> friends = genFriendNumbsToMax(max);
	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();// end time measurement
	// sort the vector
	std::cout << "Sorting...\n";
	std::sort(friends.begin(), friends.end());
	//output the pairs
	std::move(friends.begin(), friends.end(), std::ostream_iterator<pair>(std::cout, "\n"));
	//output the duration
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms\n";
	std::cin.ignore();
	std::cin.get();
	return 0;
}