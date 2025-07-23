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
#include <iostream>
#include <vector>

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
#include <algorithm>
#include <iterator>

int main() {
	uint n;
	Fast::cin >> n;

	auto pairs = Fast::cin.to_vec<std::pair<int, int>>(n);
	std::ranges::sort(pairs);
	std::vector<int> values(n);
	for (uint i = 0; i < n; i++)
		values[i] = pairs[i].second;

	std::vector<uint> insert_positions(n);

	std::vector<int> lis;
	uint peak;

	lis.push_back(values[0]);
	insert_positions[0] = 0;

	uint max_length = 0;

	for (uint i = 1; i < n; i++) {
		int val = values[i];
		do {
			if (lis[lis.size() - 1] < val) {
				insert_positions[i] = lis.size();
				lis.push_back(val);
				continue;
			}

			auto ptr = std::ranges::lower_bound(lis, val);
			insert_positions[i] = ptr - lis.begin();
			*ptr = val;
		} while (false);

		if (lis.size() > max_length) {
			max_length = lis.size();
			peak = i;
		}

	}

	std::vector<uint> result;
	for (uint i = peak + 1; i < n; i++)
		result.push_back(pairs[i].first);

	uint target_pos = insert_positions[peak];
	for (uint i = peak; i <= peak; i--) {
		if (insert_positions[i] == target_pos) {
			lis[insert_positions[i]] = values[i];
			target_pos --;
		}
		else {
			result.push_back(pairs[i].first);
		}
	}
	std::ranges::sort(result);
	std::cout << result.size() << '\n';
	for (auto& val: result)
		std::cout << val << '\n';
}
