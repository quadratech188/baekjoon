#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstring>
using namespace std;

int N, K;
string board[50];
vector<int> G[302];
int edge[302][302];

int level[302];
bool bfs() {
	memset(level, 0, sizeof(level));
	level[0] = 1;
	queue<int> que;
	que.push(0);
	while (!que.empty()) {
		int current = que.front(); que.pop();
		for (auto node : G[current]) {
			if (level[node]) continue;
			if (edge[current][node]) {
				level[node] = level[current] + 1;
				que.push(node);
			}
		}
	}

	return level[6 * N + 1];
}

int turn[302];
int dfs(int current, int flow) {
	if (current == 6 * N + 1) return flow;
	int cursize = G[current].size();
	for (int& i = turn[current]; i < cursize; i++) {
		int node = G[current][i];
		if (edge[current][node] && level[node] == level[current] + 1) {
			int f = dfs(node, min(edge[current][node], flow));
			if (f) {
				edge[current][node] -= f;
				edge[node][current] += f;
				return f;
			}
		}
	}
	return 0;
}

bool func (int X) {
	memset(edge, 0, sizeof(edge));
	for (int r = 0; r < N; r++) {
		edge[0][r + 1] = X;
		edge[r + 1][N + r + 1] = 1000000007;
		edge[r + 1][2 * N + r + 1] = K;
		edge[3 * N + r + 1][6 * N + 1] = X;
		edge[4 * N + r + 1][3 * N + r + 1] = 1000000007;
		edge[5 * N + r + 1][3 * N + r + 1] = K;
		for (int c = 0; c < N; c++) {
			if (board[r][c] == '1') edge[N + r + 1][4 * N + c + 1] = 1;
			else edge[2 * N + r + 1][5 * N + c + 1] = 1;
		}
	}

	int flow = 0;

	while (bfs()) {
		memset(turn, 0, sizeof(turn));
		int f;
		while (f = dfs(0, 1000000007)) flow += f;
	}

	return flow == N * X;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> K;
	for (int i = 0; i < N; i++) cin >> board[i];
	for (int r = 0; r < N; r++) {
		G[0].emplace_back(r + 1);
		G[r + 1].emplace_back(0);
		G[r + 1].emplace_back(N + r + 1);
		G[N + r + 1].emplace_back(r + 1);
		G[r + 1].emplace_back(2 * N + r + 1);
		G[2 * N + r + 1].emplace_back(r + 1);
		G[6 * N + 1].emplace_back(3 * N + r + 1);
		G[3 * N + r + 1].emplace_back(6 * N + 1);
		G[3 * N + r + 1].emplace_back(4 * N + r + 1);
		G[4 * N + r + 1].emplace_back(3 * N + r + 1);
		G[3 * N + r + 1].emplace_back(5 * N + r + 1);
		G[5 * N + r + 1].emplace_back(3 * N + r + 1);
		for (int c = 0; c < N; c++) {
			if (board[r][c] == '1') {
				G[N + r + 1].emplace_back(4 * N + c + 1);
				G[4 * N + c + 1].emplace_back(N + r + 1);
			}
			else {
				G[2 * N + r + 1].emplace_back(5 * N + c + 1);
				G[5 * N + c + 1].emplace_back(2 * N + r + 1);
			}
		}
	}

	int L = 0, R = 50;
	while (L < R) {
		int mid = (L + R) / 2 + 1;
		if (func(mid)) L = mid;
		else R = mid - 1;
	}

	cout << L;
}
