#include <string>
#include <iostream>

#define INF 10000
#define NIL 20000
std::string a;
std::string b;

int cache[1001][1001] = {NIL};

int distance(int index_a, int index_b) {
    if (index_a == a.size()) {
        if (index_b == b.size())
            return 0;
        else
            return 1;
	}

    if (cache[index_a][index_b] != NIL)
        return cache[index_a][index_b];

    int result = INF;

    for (int next_index_b = index_b; next_index_b < b.size(); next_index_b++) {
        if (a[index_a] != b[next_index_b])
            continue;

        int next_distance = distance(index_a + 1, next_index_b + 1);

        if (next_index_b == index_b)
            result = std::min(result, next_distance);
        else
            result = std::min(result, next_distance + 1);
	}
    
    cache[index_a][index_b] = result;
    return result;
}


int main() {
	for (int i = 0; i < 1001; ++i) {
		for (int j = 0; j < 1001; ++j) {
			cache[i][j] = NIL;
		}
	}

	std::getline(std::cin, a);
	std::getline(std::cin, b);

	int result = distance(0, 0);

	if (result == INF)
		printf("-1");
	else
		printf("%d", result);
}
