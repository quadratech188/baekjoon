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
int main() {
	int64_t n, a, b, c, d, e, f;
	Fast::cin >> n >> a >> b >> c >> d >> e >> f;

	if (n == 1) {
		std::cout << a + b + c + d + e + f - std::max({
				a, b, c, d, e, f
				});
		return 0;
	}

	int64_t face = std::min({
			a,
			b,
			c,
			d,
			e,
			f
			});
	
	int64_t edge = std::min({
			a + b,
			b + f,
			f + e,
			e + a,
			e + d,
			d + b,
			b + c,
			c + e,
			d + a,
			a + c,
			c + f,
			f + d
			});

	int64_t corner = std::min({
			e + d + a,
			d + a + b,
			b + a + c,
			c + e + a,
			d + b + f,
			b + c + f,
			e + c + f,
			f + e + d
			});

	std::cout << ((n - 2) * (n - 2) + 4 * (n - 2) * (n - 1)) * face
		+ (4 * (n - 2) + 4 * (n - 1)) * edge
		+ 4 * corner;
}
