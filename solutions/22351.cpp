#include <iostream>
#include <string>
int main() {
	std::string s;
	std::cin >> s;

	for (int a = 1; a <= 999; a++) {
		uint ptr = 0;
		for (int i = a;;i++) {
			std::string part = std::to_string(i);

			if (!std::string_view(s).substr(ptr).starts_with(part)) {
				break;
			}
			ptr += part.size();
			if (ptr == s.size()) {
				std::cout << a << ' ' << i;
				return 0;
			}
		}
	}
}

