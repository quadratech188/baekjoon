#include <bits/stdc++.h>
using namespace std;
const int MOD = 1000000007;
struct Matrix{
	int row, column;
	vector <vector<long long>> myM;
	Matrix(int row=0, int column=0){
		this->row = row;
		this->column = column;
		myM.resize(row);
		for(int i=0; i<row; i++)
			myM[i].resize(column);
	}
	Matrix operator*(Matrix m){
		if(column != m.row){
			cout << -1;
			exit(0);
		}
		Matrix result(row, m.column);
		for(int i=0; i<row; i++){
			for(int j=0; j<m.column; j++){
				long long &ref = result.myM[i][j];
				ref = 0;
				for(int k=0; k<m.row; k++){
					ref = (ref + myM[i][k] * m.myM[k][j])%MOD;
				}
			}
		}
		return result;
	}
	void scan(){
		for(int i=0; i<row; i++){
			for(int j=0; j<column; j++){
				cin >> myM[i][j];
			}
		}
	}
};

long long n, m;
int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> n >> m;
	vector<Matrix> V;
	Matrix M(m,m);
	M.myM[0][0] = M.myM[0][m-1] = 1;
	for(int i=1; i<m; ++i)
		M.myM[i][i-1] = 1;
	V.push_back(M);

	for(int i=1; i<60; ++i)
		V.push_back(V.back()*V.back());

	Matrix ans(m,m);
	for(int i=0; i<m; ++i)
		ans.myM[i][i] = 1;

	for(int i=0; i<60; ++i)
		if(n&(1LL<<i))
			ans = ans * V[i];

	cout << ans.myM[0][0] << ' ';
}
