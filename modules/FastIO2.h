#include <cstdio>
#include <istream>
#include <type_traits>

namespace Fast {
	class istream {
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar_unlocked();
			} while (ch == ' ' || ch == '\n');

			do {
				val = 10 * val + ch - '0';
				ch = getchar_unlocked();
			} while ('0' <= ch && ch <= '9');

			return *this;
		}
	};

	istream cin;
}
