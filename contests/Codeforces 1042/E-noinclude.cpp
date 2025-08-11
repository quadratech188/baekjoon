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
		std::tuple<Ts...> get() {
			std::tuple<Ts...> result;

			std::apply([this](auto&... x) {(((*this) >> x), ...);}, result);

			return result;
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

bool loop() {
	uint n;
	Fast::cin >> n;

	auto a = Fast::cin.to_vec<int>(n);
	auto b = Fast::cin.to_vec<int>(n);

	if (a[n - 1] != b[n - 1]) return false;

	for (uint i = 0; i < n - 1; i++) {
		if (b[i] != a[i] && b[i] != (a[i] ^ a[i + 1]) && b[i] != (a[i] ^ b[i + 1]))
			return false;
	}

	return true;
}

int main() {
	uint t;
	Fast::cin >> t;

	for (uint i = 0; i < t; i++) {
		if (loop())
			std::cout << "YES\n";
		else
			std::cout << "NO\n";
	}
}
