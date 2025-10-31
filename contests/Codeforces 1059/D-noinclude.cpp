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

int query(char type, uint l, uint r) {
	std::cout << type << ' ' << l + 1 << ' ' << r << std::endl;
	int ans;
	Fast::cin >> ans;
	return ans;
}

void submit(uint l, uint r) {
	std::cout << "! " << l + 1 << ' ' << r << std::endl;
}

void solve(uint l, uint r, int64_t prev_sum) {
	if (l + 1 == r) {
		submit(l, r);
		return;
	}
	uint middle = (l + r) / 2;

	int64_t left_delta = query('2', l, middle) - query('1', l, middle);
	int64_t right_delta = prev_sum - left_delta;

	if (left_delta != 0 && right_delta != 0) {
		submit(middle - left_delta, middle + right_delta);
		return;
	}

	if (left_delta != 0) {
		return solve(l, middle, left_delta);
	}
	if (right_delta != 0) {
		return solve(middle, r, right_delta);
	}
}

void loop() {
	uint n;
	Fast::cin >> n;
	int64_t total_sum = query('2', 0, n) - query('1', 0, n);
	solve(0, n, total_sum);
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) loop();
}
