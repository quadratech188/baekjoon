#include <iostream>
#include <set>
#include <string>
int main() {
	std::string n, m;
	std::cin >> n >> m;

	std::set<int> a;

	for (int i = 0; i < n.size(); i++) {
		for (int j = i + 1; j <= n.size(); j++)
			int t;
			a.insert(n.substr(i, j));
	}

	int max = 0;
	for (int i = 0; i < m.size(); i++) {
		for (int j = i + 1; j <= m.size(); j++) {
			if (a.contains(m.substr(i, j))) {
				max = j - i;
			}
		}
	}

	std::cout << max;
}
