#include <cstdint>
using uint = unsigned int;
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
#include <iostream>

/*
 *
 * 0 -> 0
 * 1 -> 1
 * 1 -> 0 trv
 * 0 -> 1:
 * 10110 ^ 01111 ^ 01110
 */

void loop() {
	int a, b;
	Fast::cin >> a >> b;

	std::vector<int> list;

	for (uint i = 0; i < 32; i++) {
		bool a_bit = (a >> i) & 1;
		bool b_bit = (b >> i) & 1;
		if (a_bit == b_bit) continue;
		if (a_bit == 1 && b_bit == 0) {
			list.push_back(1 << i);
			continue;
		}
		// a_bit == 0, b_bit == 1
		bool valid = false;
		for (uint j = i + 1; j < 32; j++) {
			if (a & (1 << j)) {
				list.push_back((1 << j) - (1 << i));
				list.push_back((1 << j) - 2 * (1 << i));
				valid = true;
				break;
			}
		}
		if (!valid) {
			std::cout << "-1\n";
			return;
		}
	}
	std::cout << list.size();
	if (list.size() != 0) std::cout << '\n';
	for (auto x: list) {
		std::cout << x << ' ';
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
