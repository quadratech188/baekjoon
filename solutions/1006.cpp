#include "../modules/FastIO2.h"
#include "../modules/ListGraph2.h"
#include "../modules/Matrix.h"
#include <sys/types.h>

void loop() {
	uint n;
	int w;
	Fast::cin >> n >> w;

	Matrix<int> values(n, 2);

	for (Int2 index: values.bounds())
		Fast::cin >> values[index];

	ListGraph<std::monostate, int>
		::with_reversible<true>
		::with_index<uint> graph(n);
}

int main() {
	int t;
	Fast::cin >> t;
	for (int i = 0; i < t; i++) loop();
}
