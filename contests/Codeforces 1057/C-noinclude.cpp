#include <cstdint>
using uint = unsigned int;
#include <algorithm>
#include <iostream>
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
#include <map>
#include <numeric>

void loop() {
	uint n;
	Fast::cin >> n;

	std::map<int, uint> stick_counts;

	for (int len: Fast::cin.to_range<int>(n)) {
		stick_counts[len] ++;
	}

	std::vector<int> sym_lens, odd_lens;
	for (auto [len, cnt]: stick_counts) {
		for (uint i = 0; i < cnt / 2; i++) {
			sym_lens.push_back(len);
		}
		if (cnt % 2 == 1) {
			odd_lens.push_back(len);
		}
	}

	if (sym_lens.size() == 0) {
		std::cout << "0\n";
		return;
	}

	int64_t sum = std::accumulate(sym_lens.begin(), sym_lens.end(), int64_t(0));

	int64_t result = 0;
	if (sym_lens.size() != 1) {
		result = 2 * sum;
	}

	std::ranges::sort(odd_lens);
	if (odd_lens.size() >= 1) {
		for (int i = odd_lens.size() - 1; i >= 0; i --) {
			if (odd_lens[i] < 2 * sum) {
				result = std::max(result, 2 * sum + odd_lens[i]);
				break;
			}
		}
	}
	if (odd_lens.size() >= 2) {
		for (uint i = odd_lens.size() - 1; i > 0; i --) {
			if (odd_lens[i] - odd_lens[i - 1] < 2 * sum) {
				result = std::max(result, 2 * sum + odd_lens[i] + odd_lens[i - 1]);
				break;
			}
		}
	}
	std::cout << result << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
