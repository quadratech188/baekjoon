#include <ios>
#include <iostream>
#include <vector>

int const MOD = 1'000'000'007;

struct mint {
	int val;

	mint operator+(mint const& other) const {
		return {(val + other.val) % MOD};
	}

	mint operator*(mint const& other) const {
		return {(int)((long long int)val * other.val % MOD)};
	}

	mint operator-(int const& other) const {
		if (other > val)
			return {val - other + MOD};
		return {val - other};
	}
};

std::vector<int> A, B;
std::vector<mint> size, sum, sum_left, sum_right;

void solve(int root) {
	if (root == 0) return;

	if (size[root].val != -1) return;

	int l = A[root];
	int r = B[root];

	solve(l);
	solve(r);

	size[root] = size[l] + size[r];

	sum[root] = sum[l] + sum[r] + sum_right[l] * size[r] + sum_left[r] * size[l] - 1;

	sum_left[root] = sum_left[l] + size[r] + sum_left[r] - 1;

	sum_right[root] = sum_right[r] + size[l] + sum_right[l] - 1;
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout.tie(0);

	int n;
	std::cin >> n;
	A.resize(n + 1);
	B.resize(n + 1);
	size.resize(n + 1, {-1});
	sum.resize(n + 1);
	sum_left.resize(n + 1);
	sum_right.resize(n + 1);

	for (int i = 1; i <= n; i++)
		std::cin >> A[i] >> B[i];

	size[0] = {1};
	sum[0] = {1};
	sum_left[0] = {1};
	sum_right[0] = {1};

	for (int i = 1; i <= n; i++) {
		solve(i);
		std::cout << sum[i].val << '\n';
	}
}
