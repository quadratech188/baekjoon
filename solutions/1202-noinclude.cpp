#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <queue>
#include <type_traits>
#include <unistd.h>
#include <utility>
#include <vector>
template <typename T>
struct no_init {

	T value;

	no_init() {}

	no_init(T value):
		value(value) {}

	T& val() {
		return value;
	}

	no_init& operator=(T&& other) {
		value = other;
		return *this;
	}

	operator T() {
		return value;
	}
};

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

	/*
	class ostream {
		private:
			inline void putchar(char const& ch) {
				static char buffer[FASTOSTREAM_BUFFER_SIZE];
				static char* ptr = buffer;
				static char* end = buffer + (FASTOSTREAM_BUFFER_SIZE);

				if (ptr == end) {
					write(STDOUT_FILENO, buffer, FASTOSTREAM_BUFFER_SIZE);
					ptr = buffer;
				}
				*(ptr++) = ch;
			}
		public:
			template <typename T>
				inline ostream& operator<<(T& val)
				requires std::is_integral_v<T> {
					if (val < 0)
						putchar('-');
				}
	};
	*/
}

int main() {
	uint n, k;
	Fast::cin >> n >> k;

	auto jewels = Fast::cin.to_vec<std::pair<int, int>>(n);
	std::ranges::sort(jewels);

	auto bags = Fast::cin.to_vec<int>(k);
	std::ranges::sort(bags);

	std::priority_queue<int> queue;

	int64_t result = 0;

	uint ptr = 0;
	for (auto const& bag: bags) {
		for (; ptr < n; ptr++) {
			if (jewels[ptr].first > bag) break;
			queue.push(jewels[ptr].second);
		}

		if (!queue.empty()) {
			result += queue.top();
			queue.pop();
		}
	}

	std::cout << result;
}
