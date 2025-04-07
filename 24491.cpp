#include <iostream>
void loop() {
	int a, b;
	std::cin >> a >> b;

	for (int i = -18; i <= 18; i++) {
		int a1 = a << i;
		if (a1 > b) continue;
	}
}

int main() {
	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++) loop();
}
