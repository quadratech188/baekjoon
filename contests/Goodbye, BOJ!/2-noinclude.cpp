#include <iostream>
#include <vector>

struct Problem {
	long long a, b, c, p;
};

void solve() {
	long long n;
	std::cin >> n;

	std::vector<Problem> problems;

	for (long long i = 0; i < n; i++) {
		long long a, b, c, p;
		std::cin >> a >> b >> c >> p;

		problems.emplace_back(a, b, c, p);
	}
	long long budget = 0;
	long long prev_time = 0;
	long long a_min = 0, b_min = 0, c_min = 0;

	for (auto problem: problems) {
		budget += (problem.p - prev_time - 1);

		long long a_new = std::max(a_min, problem.a);
		long long b_new = std::max(b_min, problem.b);
		long long c_new = std::max(c_min, problem.c);

		long long pred_cost = (a_new + b_new + c_new - a_min - b_min - c_min);
		if (pred_cost > budget) {
			std::cout << "NO\n";
			return;
		}
		budget -= pred_cost;
		a_min = a_new;
		b_min = b_new;
		c_min = c_new;
		prev_time = problem.p;
	}
	std::cout << "YES\n";
}

int main() {
	std::cin.tie(nullptr);
	std::ios_base::sync_with_stdio(false);
	std::cout.tie(nullptr);

	long long t;
	std::cin >> t;
	for (long long i = 0; i < t; i++) solve();
}
