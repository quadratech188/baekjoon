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
#include <algorithm>
#include <cstdint>

int main() {
	int64_t t;
	Fast::cin >> t;
	uint n;
	Fast::cin >> n;
	auto as = Fast::cin.to_vec<int>(n);

	std::vector<int64_t> a_sums;
	a_sums.reserve(n * (n + 1) / 2);

	for (uint i = 0; i < n; i++) {
		int64_t sum = 0;
		for (uint j = i; j < n; j++) {
			sum += as[j];
			a_sums.push_back(sum);
		}
	}
	std::ranges::sort(a_sums);

	uint m;
	Fast::cin >> m;
	auto bs = Fast::cin.to_vec<int>(m);

	std::vector<int64_t> b_sums;
	b_sums.reserve(m * (m + 1) / 2);

	for (uint i = 0; i < m; i++) {
		int64_t sum = 0;
		for (uint j = i; j < m; j++) {
			sum += bs[j];
			b_sums.push_back(sum);
		}
	}
	std::ranges::sort(b_sums);

	int64_t cnt = 0;
	for (auto& val: a_sums) {
		cnt += std::ranges::upper_bound(b_sums, t - val) - std::ranges::lower_bound(b_sums, t - val);
	}

	std::cout << cnt;
}
