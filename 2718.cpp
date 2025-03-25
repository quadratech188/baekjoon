#include "modules/Matrix.h"
#include <iostream>

int solution(Matrix<int>& cache, int state, int n) {
	if (cache(state, n) != -1)
		return cache(state, n);

	switch (state) {

	}
}

void loop() {
	int n;
	std::cin >> n;
	// 1
	// 2
	// 4
	// 8
	Matrix<int> cache(15, n, -1);

	for (int i = 0; i < 15; i++)
		cache(i, 1) = 0;
	cache(0, 1) = 1;
	cache(3, 1) = 1;
	cache(12, 1) = 1;
	cache(9, 1) = 1;
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
