// #include "videoclips.h"
#include "modules/SparseTable.h"

int k, m;
SparseTable table;

void videos(int K, int M, int S[]) {
	SparseTable table(K, M, [&S](int index) {
			return S[index];
			});
	m = M;
}

int clip(int I) {
	return table.after(I, m);
}
