#include <cstdio>
#include <istream>

class FastIstream: public std::istream {
public:
	FastIstream& operator>>(int& val) {
		char ch;

		do {
			ch = std::getchar();
		} while (ch == ' ' || ch == '\n');

		int result = 0;

		do {
			result = 10 * result + ch - '0';
			ch = std::getchar();
		} while (ch != ' ' && ch != '\n');

		val = result;

		return *this;
	}
};

FastIstream FastCin;
