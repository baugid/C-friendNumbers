#include <iostream>
#include <vector>

struct pair {
	unsigned long long val1;
	unsigned long long val2;
};

unsigned long long sumDividers(const unsigned long long& n) {
	unsigned long long sum = 1;
	unsigned long long iSqare;
	for (unsigned long long i = 2; i < n; i++) {
		if (n%i==0) {
			iSqare = i*i;
				if (iSqare < n) {
					sum += i;
					sum += n / i;
				}
				else {
					if (iSqare == n) {
						sum += i;
					}
					return sum;
				}
		}
	}
	return sum;
}

std::vector<pair> genFriendNumbsToMax(const unsigned long long& max){
	std::vector<pair> out;
	unsigned long long dividerSum;
	for (unsigned long long i = 2; i <= max; i++) {
		dividerSum = sumDividers(i);
		if (i < dividerSum&&i == sumDividers(dividerSum)) {
			out.push_back({ i,dividerSum });
		}
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
	std::vector<pair> friends = genFriendNumbsToMax(max);
	for (size_t i = 0; i < friends.size(); i++) {
		std::cout << friends[i].val1 << ", " << friends[i].val2 << "\n";
	}
	std::cin.ignore();
	std::cin.get();
}