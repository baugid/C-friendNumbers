#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <algorithm>
#define CORE_COUNT 16
//pair of two unsigned long longs ordered after the first number
struct pair {
	unsigned long long val1;
	unsigned long long val2;

	bool operator<(pair other) {
		return (val1 < other.val1);
	}
};

unsigned long long sumDividers(const unsigned long long& n) {
	unsigned long long sum = 1;
	unsigned long long root =sqrtl(n);
	//add all divisors <sqrt(n) to sum
	for (unsigned long long i = 2; i < root; i++) {
		if (n%i==0) { 
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

std::vector<pair> genFriendNumbsToMax(const unsigned long long& max){
	std::vector<pair> out;
	std::mutex lock;
	std::thread threads[CORE_COUNT]; // Create as much threads as wanted
	for (int i = 0; i < CORE_COUNT; i++) {
		threads[i] = std::thread([&out,&lock,min= i == 0 ? 2 : max / CORE_COUNT * i,threadMax = i == CORE_COUNT - 1 ? max : max / CORE_COUNT * (i + 1) - 1]{ // min is 1/15*i but zero and one should be left out the last thread must test the whole rest
			for (unsigned long long i = min; i <= threadMax; i++) {
				unsigned long long dividerSum = sumDividers(i);
				if (i < dividerSum&&i == sumDividers(dividerSum)) {//pairs shouldn´t occur twice, check if i has a friend number
					//lock and add the friend pair
					lock.lock();
					out.push_back({ i,dividerSum });
					lock.unlock();
				}
			}
		});
	}
	//join all threads
for (int i = 0; i < CORE_COUNT; i++) {
		threads[i].join();
}
	return out;
}

int main() {
	bool error;
	unsigned long long max;
	//fetch user input
	do {
		error = false;
		std::cout << "Enter maximum value: ";
		std::cin >> max;
		if (max <= 2) {
			std::cout << "Maximum value must be greater than one!\n";
		}
	} while (error);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();//start time measurement
	std::vector<pair> friends = genFriendNumbsToMax(max);
	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();// end time measurement
	std::sort(friends.begin(), friends.end()); // sort the vector
	//output the pairs
	for (size_t i = 0; i < friends.size(); i++) {
		std::cout << friends[i].val1 << ", " << friends[i].val2 << "\n";
	}
	//output the duration
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << "S\n";
	std::cin.ignore();
	std::cin.get();
}