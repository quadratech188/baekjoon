#include <algorithm>
#include <limits>
#include <string>
std::string string;

int solution(bool used_4) {
	int min = std::numeric_limits<int>::max();

	if (!used_4) {
		for (int i = 0; i < string.size(); i++) {
			for (int j = i + 1; j < string.size(); j++) {
				std::swap(string[i], string[j]);
				min = std::min(min, solution(true) + 1);
			}
		}
	}
}

int main() {

}
