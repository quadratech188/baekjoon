#include "../modules/Matrix.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ios>

int get(Int2 index) {
	if (index.x >= 0 && -index.x <= index.y && index.y < index.x)
		return 4 * index.x * index.x - 3 * index.x + 1 - index.y;

	else if (index.y <= 0 && index.y <= index.x && index.x < -index.y)
		return 4 * index.y * index.y + index.y + 1 - index.x;

	else if (index.x <= 0 && index.x < index.y && index.y <= -index.x)
		return 4 * index.x * index.x - index.x + 1 + index.y; 

	else
	 	return 4 * index.y * index.y + 3 * index.y + 1 + index.x;
}

int main() {
	Int2 begin, end;
	std::cin >> begin.y >> begin.x >> end.y >> end.x;
	Matrix<int> values(end.x - begin.x + 1, end.y - begin.y + 1);

	int max = 0;

	for (Int2 index: values.bounds()) {
		values[index] = get(index + begin);
		max = std::max(max, values[index]);
	}

	int len = std::floor(std::log10(max)) + 1;

	for (Int2 index: values.bounds()) {
		std::cout << std::setw(len) << values[index] << ' ';

		if (index.x == values.size().x - 1) std::cout << '\n';
	}
}
