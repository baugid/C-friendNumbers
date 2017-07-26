#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <algorithm>
#define CORE_COUNT 16

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
	for (unsigned long long i = 2; i < root; i++) {
		if (n%i==0) {
			sum += i;
			sum += n / i;
		}
	}
	if (root*root == n) {
		sum += root;
	}
	return sum;
}

std::vector<pair> genFriendNumbsToMax(const unsigned long long& max){
	std::vector<pair> out;
	std::mutex lock;
	std::thread threads[CORE_COUNT];
	for (int i = 0; i < CORE_COUNT; i++) {
		threads[i] = std::thread([&out,&lock,min= i == 0 ? 2 : max / CORE_COUNT * i,threadMax = i == CORE_COUNT - 1 ? max : max / CORE_COUNT * (i + 1) - 1]{
			for (unsigned long long i = min; i <= threadMax; i++) {
				unsigned long long dividerSum = sumDividers(i);
				if (i < dividerSum&&i == sumDividers(dividerSum)) {
					lock.lock();
					out.push_back({ i,dividerSum });
					lock.unlock();
				}
			}
		});
	}

for (int i = 0; i < CORE_COUNT; i++) {
		threads[i].join();
}
	return out;
}

int main() {
	bool error;
	unsigned long long max;
	do {
		error = false;
		std::cout << "Enter maximum value: ";
		std::cin >> max;
		if (max <= 2) {
			std::cout << "Maximum value must be greater than one!\n";
		}
	} while (error);
	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
	std::vector<pair> friends = genFriendNumbsToMax(max);
	std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
	std::sort(friends.begin(), friends.end());
	for (size_t i = 0; i < friends.size(); i++) {
		std::cout << friends[i].val1 << ", " << friends[i].val2 << "\n";
	}
	std::cout << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << "S\n";
	std::cin.ignore();
	std::cin.get();
}