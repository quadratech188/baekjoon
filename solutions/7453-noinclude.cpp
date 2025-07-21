#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <istream>
#include <type_traits>
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

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& values) {
	if (values.size() == 0) return os;

	for (size_t i = 0; i < values.size() - 1; i++) {
		std::cout << values[i] << ' ';
	}
	std::cout << values[values.size() - 1];

	return os;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& pair) {
	std::cout << pair.first << ' ' << pair.second;

	return os;
}

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<int> a(n), b(n), c(n), d(n);

	for (uint i = 0; i < n; i++)
		Fast::cin >> a[i] >> b[i] >> c[i] >> d[i];

	std::vector<int> ab, cd;
	ab.reserve(n * n);
	cd.reserve(n * n);

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++) {
			ab.push_back(a[i] + b[j]);
			cd.push_back(c[i] + d[j]);
		}
	}

	std::ranges::sort(ab);
	std::ranges::sort(cd, std::greater());

	std::vector<std::pair<int, int>> ab_s;
	uint ptr = 0;
	while (ptr < ab.size()) {
		uint i;
		for (i = ptr + 1; i < ab.size(); i++) {
			if (ab[i] != ab[ptr]) break;
		}
		ab_s.emplace_back(ab[ptr], i - ptr);
		ptr = i;
	}

	std::vector<std::pair<int, int>> cd_s;
	ptr = 0;
	while (ptr < cd.size()) {
		uint i;
		for (i = ptr + 1; i < cd.size(); i++) {
			if (cd[i] != cd[ptr]) break;
		}
		cd_s.emplace_back(cd[ptr], i - ptr);
		ptr = i;
	}

	uint l_ptr = 0;
	uint r_ptr = 0;

	int64_t cnt = 0;
	while (l_ptr != ab_s.size() && r_ptr != cd_s.size()) {
		int sum = ab_s[l_ptr].first + cd_s[r_ptr].first;
		if (sum == 0) cnt += (int64_t)ab_s[l_ptr].second * cd_s[r_ptr].second;

		if (l_ptr == ab_s.size() - 1) {
			r_ptr++;
			continue;
		}
		if (r_ptr == cd_s.size() - 1) {
			l_ptr++;
			continue;
		}

		if (sum < 0)
			l_ptr++;
		else
		 	r_ptr++;
	}

	std::cout << cnt;
}
