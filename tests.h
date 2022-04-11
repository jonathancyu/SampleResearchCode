#pragma once
#include <string>
#include <vector>
#include <iostream>
class tests {
public:
	struct test {
		test(std::string n, bool (*f)()) {
			name = n;
			func = f;
		}
		std::string name;
		bool (*func)();
	};

	std::vector<test> items;

	tests();
	void run() {
		for (test t : items) {
			bool result = t.func();
			std::cout << t.name << ": " << boolToString(result) << std::endl;
		}
	}

	std::string boolToString(bool b) {
		return (b ? "pass" : "FAIL");
	}

	static int compareVector(std::vector<double> expected, std::vector<double> result, double tolerance) {
		int i = 0;
		auto e = expected.begin();
		auto r = result.begin();
		while (e != expected.end() && r != result.end()) {
			//if (*e != *r) {
			if (abs(*e-*r) >= tolerance) {
				std::cout << i << std::endl;
				return i;
			}
			i++;
			e++;
			r++;
		}
		return -1;
	}

	static int compare2dVector(std::vector<std::vector<double>> expected, std::vector<std::vector<double>> result, double tolerance) {
		int i = 0;
		auto e = expected.begin();
		auto r = result.begin();
		while (e != expected.end() && r != result.end()) {
			if (compareVector(*e, *r, tolerance) != -1) {
				return i;
			}
			i++;
			e++;
			r++;
		}
		return -1;
	}


};

