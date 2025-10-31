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
#include <functional>
#include <iostream>

void loop() {
	uint n, k;
	Fast::cin >> n >> k;

	auto values = Fast::cin.to_vec<int>(n);

	std::vector<bool> contained(n);
	for (auto v: values) {
		contained[v - 1] = true;
	}
	if (std::ranges::all_of(contained, std::identity{})) {
		for (uint i = 0; i < k; i++) {
			std::cout << values[i] << ' ';
		}
		std::cout << '\n';
		return;
	}

	int unused = std::ranges::find(contained, false) - contained.begin();
	std::cout << unused + 1 << ' ';
	if (k == 1) {
		std::cout << '\n';
		return;
	}

	if (k == 2) {
		for (int i = 0; i < n; i++) {
			if (i != unused && i != values.back() - 1) {
				std::cout << i + 1 << '\n';
				break;
			}
		}
		return;
	}

	std::vector<int> result;

	int ptr = n - 1;
	for (uint i = 0; i < k - 1; i++) {
		if (unused == ptr) ptr --;
		result.push_back(ptr);
		ptr --;
	}

	if (result.front() == values.back() - 1) {
		std::swap(result[0], result[1]);
	}
	for (auto x: result) {
		std::cout << x + 1 << ' ';
	}
	std::cout << '\n';
}

int main() {
	uint t;
	Fast::cin >> t;
	for (uint i = 0; i < t; i++) loop();
}
