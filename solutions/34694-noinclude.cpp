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
#include <algorithm>
#include <iostream>
#include <vector>
uint a, b, w, M;
std::vector<int> flexible_classes;
std::vector<int> max_heights;

int main() {
	Fast::cin >> a >> b >> w >> M;
	flexible_classes = Fast::cin.to_vec<int>(a);
	max_heights.resize(w);
	for (auto& c: max_heights) {
		c = std::ranges::max(Fast::cin.to_vec<int>(b));
	}

	std::ranges::sort(flexible_classes, std::greater{});
	std::ranges::sort(max_heights, std::greater{});

	int64_t result = 0;
	for (uint i = 0; i < w; i++) {
		int max_height = max_heights[i];
		if ((M - b) * i < a) {
			max_height = std::max(max_height, flexible_classes[(M - b) * i]);
		}
		result += 2 * max_height - 2;
	}
	std::cout << result << '\n';
}
