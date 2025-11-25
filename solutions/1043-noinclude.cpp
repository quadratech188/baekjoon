#include <ranges>
#include <unistd.h>
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
		char buffer[FASTISTREAM_BUFFER_SIZE];
		char* ptr = buffer;
		char* end = buffer;

		inline char getchar() {
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

		template <typename... Ts>
		inline istream& operator>>(std::tuple<Ts...>& tuple){
			std::apply([this](auto&... x) {(((*this) >> x), ...);}, tuple);
			return *this;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}

		template <typename T>
		auto to_range(uint size) {
			return std::views::iota(0u, size)
				| std::views::transform([this](uint) {
						T temp;
						(*this) >> temp;
						return temp;
						});
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
#include <algorithm>
#include <iostream>

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	UnionFind uf(n + 1);

	uint cnt;
	Fast::cin >> cnt;

	for (auto x: Fast::cin.to_range<uint>(cnt)) {
		uf.connect(0, x);
	}

	std::vector<uint> firsts;

	for (uint i = 0; i < m; i++) {
		uint first, cnt;
		Fast::cin >> cnt >> first;
		for (auto x: Fast::cin.to_range<uint>(cnt - 1)) {
			uf.connect(first, x);
		}
		firsts.push_back(first);
	}

	uint result = 0;
	for (auto first: firsts) {
		if (!uf.connected(0, first)) {
			result ++;
		}
	}
	std::cout << result << '\n';
}
