#include <iostream>
#include <regex>

int main() {
	int t;
	std::cin >> t;

	std::regex regex("((100+1+)|(01))+");

	for (int i = 0; i < t; i++) {
		std::string buf;
		std::cin >> buf;
		
		if (std::regex_match(buf, regex))
			std::cout << "YES\n";
		else
		 	std::cout << "NO\n";
	}
}
