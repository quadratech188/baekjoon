#include <algorithm>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <vector>

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

int main() {
	FastIO();
	int n;
	std::cin >> n;

	std::vector<long long int> differences(n);
	int a_initial = 0;
	for (int i = 0; i < n; i++) {
		int a, b;
		std::cin >> a >> b;
		differences[i] = b - a;
		a_initial += a;
	}

	std::sort(differences.begin(), differences.end());

	std::vector<long long int> accumulate(n + 1);
	accumulate[0] = 0;
	for (int i = 0; i < n; i++)
		accumulate[i + 1] = accumulate[i] + differences[i];

	int q;
	std::cin >> q;

	long long int delta_a = 0, delta_b = 0;

	for (int i = 0; i < q; i++) {
		char type;
		int delta;
		std::cin >> type >> delta;
		switch(type) {
			case 'A':
				delta_a += delta;
				break;
			case 'B':
				delta_b += delta;
		}

		int start = std::distance(differences.begin(),
				std::upper_bound(differences.begin(), differences.end(), delta_a - delta_b));

		std::cout << a_initial + (delta_a - delta_b) * start + accumulate[n] - accumulate[start] + delta_b * n << '\n';
	}
}
