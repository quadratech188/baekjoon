#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}

		template <typename T1, typename T2>
		inline istream& operator>>(std::pair<T1, T2>& pair) {
			(*this) >> pair.first >> pair.second;
			return *this;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}
	};

	istream cin;
}
#include <numeric>
#include <vector>

class UnionFind {
public:
	UnionFind(size_t size) {
		next.resize(size);
		std::iota(next.begin(), next.end(), 0);
	}

	size_t find(size_t x) {
		if (next[x] == x)
			return x;
		next[x] = find(next[x]);
		return next[x];
	}

	void connect(size_t from, size_t to) {
		next[find(from)] = find(to);
	}

	bool connected(size_t x, size_t y) {
		return find(x) == find(y);
	}

	void flatten() {
		for (size_t i = 0; i < next.size(); i++)
			find(i);
	}

	size_t at(size_t index) {
		return next[index];
	}

private:
	std::vector<size_t> next;
};

int main() {
	uint n, m, k;
	Fast::cin >> n >> m >> k;

	auto candies = Fast::cin.to_vec<int>(n);

	UnionFind uf(n);

	for (uint i = 0; i < m; i++) {
		uint a, b;
		Fast::cin >> a >> b;
		uf.connect(a - 1, b - 1);
	}
	uf.flatten();

	std::vector<uint> child_count(n, 1);

	for (uint i = 0; i < n; i++) {
		if (uf.at(i) == i) continue;

		candies[uf.at(i)] += candies[i];
		child_count[uf.at(i)] ++;
	}

	std::vector<int> max_candies(k);

	for (uint i = 0; i < n; i++) {
		if (uf.at(i) != i) continue;

		for (uint children_used = k - 1; child_count[i] <= children_used; children_used --) {
			max_candies[children_used] = std::max(
					max_candies[children_used],
					max_candies[children_used - child_count[i]] + candies[i]
					);
		}
	}

	std::cout << max_candies[k - 1];
}
