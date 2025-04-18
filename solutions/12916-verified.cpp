#include <bits/stdc++.h>
#define fastio ios::sync_with_stdio(0), cin.tie(0), cout.tie(0)
using namespace std;

using matrix = vector<vector<long long>>;
const int MOD = 1e9 + 7;

matrix operator * (matrix const& a, matrix const& b) {
	matrix ret(a.size(), vector<long long>(b[0].size()));
	for (int i = 0; i < a.size(); i++) for (int j = 0; j < b.size(); j++) for (int k = 0; k < b[0].size(); k++) {
		ret[i][k] = (ret[i][k] + a[i][j] * b[j][k]) % MOD;
	}
	return ret;
}

matrix pow(matrix a, int n) {
	matrix ret(a.size(), vector<long long>(a.size()));
	for (int i = 0; i < a.size(); i++) ret[i][i] = 1;
	while (n > 0) {
		if (n & 1) ret = ret * a;
		n /= 2;
		a = a * a;
	}
	return ret;
}

int main() {
	fastio;
	int n, k; cin >> n >> k;
	matrix A(n, vector<long long>(n));
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> A[i][j];
	A = pow(A, k);
	int ans = 0;
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) ans = (ans + A[i][j]) % MOD;
	cout << ans << '\n';
}
