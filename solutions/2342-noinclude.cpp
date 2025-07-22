#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <limits>
#include <type_traits>
#include <unistd.h>
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

int lookup[5][5] = {
	{1, 2, 2, 2, 2},
	{2, 1, 3, 4, 3},
	{2, 3, 1, 3, 4},
	{2, 4, 3, 1, 3},
	{2, 3, 4, 3, 1}
};

int main() {
	std::array<std::array<int, 5>, 5> a, b;
	decltype(a)& current = a;
	decltype(a)& prev = b;

	for (uint i = 0; i < 5; i++) {
		for (uint j = 0; j < 5; j++)
			current[i][j] = std::numeric_limits<int>::max() / 2;
	}
	current[0][0] = 0;

	while (true) {
		uint type;
		Fast::cin >> type;

		if (type == 0) break;

		std::swap(current, prev);

		for (uint i = 0; i < 5; i++) {
			for (uint j = 0; j < 5; j++) {
				current[i][j] = std::numeric_limits<int>::max() / 2;

				if (i != type && j != type)
					continue;

				if (i == j)
					continue;

				if (i == type) {
					for (uint k = 0; k < 5; k++)
						current[i][j] = std::min(current[i][j], prev[k][j] + lookup[k][i]);
				}
				if (j == type) {
					for (uint k = 0; k < 5; k++)
						current[i][j] = std::min(current[i][j], prev[i][k] + lookup[k][j]);
				}
			}
		}
	}

	int min = std::numeric_limits<int>::max();

	for (uint i = 0; i < 5; i++) {
		for (uint j = 0; j < 5; j++)
			min = std::min(min, current[i][j]);
	}

	std::cout << min;
}
