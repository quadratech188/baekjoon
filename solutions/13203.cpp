#include <float.h>
#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include "modules/SegmentTree.h"
#include "modules/ModInt.h"
#include "modules/FastIO.h"

typedef ModInt<int, 1000000007> INT;

struct Data {
	INT values[6][6];
	char chr;

	Data() {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				this->values[i][j] = 0;
			}
		}
	}

	Data(char ch, std::string& name) {
		this->chr = ch;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				if (i == j) {
					this->values[i][j] = 1;
				}
				else {
					this->values[i][j] = 0;
				}
			}
		}
		for (int i = 0; i < name.size(); i++) {
			if (name[i] == ch) {
				this->values[i][i + 1] = 1;
			}
		}
	}

	Data operator+(const Data& other) {
		Data result;
		for (int a = 0; a < 6; a++) {
			for (int b = a; b < 6; b++) {
				for (int c = b; c < 6; c++) {
					result.values[a][c] += this->values[a][b] * other.values[b][c];
				}
			}
		}
		return result;
	}

	void update(std::pair<char, std::string&> action) {
		std::string& name = action.second;
		char ch = action.first;

		for (int i = 0; i < name.size(); i++) {
			if (name[i] == this->chr)
				this->values[i][i + 1] = 0;
			if (name[i] == ch)
				this->values[i][i + 1] = 1;
		}
		this->chr = ch;
	}
};

void loop() {
	int n, m, q;
	std::cin >> n >> m >> q;

	std::string str;
	std::cin >> str;
	std::string name;
	std::cin >> name;

	std::vector<Data> dataVec;
	dataVec.reserve(n);
	for (char ch: str) {
		dataVec.emplace_back(ch, name);
	}

	SegmentTree<Data, std::pair<char, std::string&>> data(dataVec);

	std::cout << data.root().values[0][m] << std::endl;

	for (int i = 0; i < q; i++) {
		int a, b;
		std::string segment;
		std::cin >> a >> b >> segment;

		for (int i = 0; i < b - a + 1; i++) {
			data.update(a - 1 + i, {segment[i], name});
		}
		std::cout << data.root().values[0][m] << std::endl;
	}
}

int main() {
	FastIO();
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		loop();
	}
}
