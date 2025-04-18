#include "modules/SegmentTree.h"
#include <stdio.h>

enum Letter {
	L,
	R
};

struct Data {
	Letter left;
	Letter right;
	int length_full;
	int length_left;
	int length_right;
	int length_floating;

	Data operator+(const Data& other) const {
		Data result;
		result.left = this->left;
		result.right = other.right;
		if (this->right != other.left) {
			if (this->length_full != 0 && other.length_full != 0) {
				result.length_full = this->length_full + other.length_full;
			}
			else {
				result.length_full = 0;
			}

			if (this->length_full != 0)
				result.length_left = this->length_full + other.length_left;
			else
				result.length_left = this->length_left;

			if (other.length_full != 0)
				result.length_right = this->length_right + other.length_full;
			else
				result.length_right = other.length_right;

			result.length_floating = std::max(std::max(this->length_floating, other.length_floating), this->length_right + other.length_left);
		}
		else {
			result.length_full = 0;
			result.length_left = this->length_left;
			result.length_right = other.length_right;
			result.length_floating = std::max(this->length_floating, other.length_floating);
		}
		return result;
	}

	void update(bool action) {
		if (this->left == L) {
			this->left = R;
			this->right = R;
		}
		else {
			this->left = L;
			this->right = L;
		}
	}
};

int main() {
	int n, q;
	scanf("%d %d", &n, &q);
	
	SegmentTree<Data, bool> tree(n, {
			.left = L,
			.right = L,
			.length_full = 1,
			.length_left = 1,
			.length_right = 1,
			.length_floating = 1
			});

	for (int i = 0; i < q; i++) {
		size_t index;
		scanf("%zu", &index);
		tree.update(index - 1, true);
		printf("%d\n", tree.root().length_floating);
	}
}
