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
#include <algorithm>
#include <iostream>
#include <map>

void loop() {
	uint n;
	Fast::cin >> n;

	std::map<int, uint> counts;

	for (int a: Fast::cin.to_range<int>(n)) {
		counts[a] ++;
	}

	std::vector<std::pair<uint, int>> pairs;

	for (auto [key, count]: counts) {
		pairs.emplace_back(count, key);
	}

	std::ranges::sort(pairs, std::greater{});

	int64_t alice_points = 0;
	int64_t bob_points = 0;
	bool bobs_turn = false;

	for (auto [count, v]: pairs) {
		if (v % 2 == 0) {
			alice_points += ((int64_t)v / 2) * count;
			bob_points += ((int64_t)v / 2) * count;
		}
		else {
			if (bobs_turn) {
				bob_points += ((int64_t)v / 2 + 1) * count;
				alice_points += ((int64_t)v / 2) * count;
				bobs_turn = false;
			}
			else {
				alice_points += ((int64_t)v / 2 + 1) * count;
				bob_points += ((int64_t)v / 2) * count;
				bobs_turn = true;
			}
		}
	}

	std::cout << alice_points << ' ' << bob_points << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
