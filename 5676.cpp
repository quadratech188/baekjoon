#include "modules/SegTree.cpp"
#include <vector>
#include <stdio.h>

std::vector<int> values;

int combine_func(int val1, int val2) {
	return val1 * val2;
}

int value_func(int index) {
	if (values[index] > 0) {
		return 1;
	}
	else if (values[index] == 0) {
		return 0;
	}
	else {
		return -1;
	}
}


int main() {
	while (true) {
		int n, k;

		int isEOF = scanf("%d %d", &n, &k);

		if (isEOF == -1) {
			break;
		}

		values = std::vector<int>(n);

		for (int i = 0; i < n; i++) {
			scanf("%d", &values[i]);
		}
		
		auto root = SegmentNode<int, int>({0, n}, combine_func, value_func);

		for (int i = 0; i < k; i++) {
			char c;
			int a, b;
			scanf(" %c %d %d", &c, &a, &b);

			if (c == 'P') {
				int result = root.query({a - 1, b});
					
				if (result == 1) {
					printf("+");
				}				
				else if (result == 0) {
					printf("0");
				}
				else {
					printf("-");
				}
			}
			else {
				values[a - 1] = b;
				root.update(a - 1);
			}
		}
		printf("\n");
	}
	return 0;
}
