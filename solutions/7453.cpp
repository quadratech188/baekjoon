#include "../modules/FastIO2.h"
#include "../modules/Output.h"
#include <algorithm>
#include <vector>

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<int> a(n), b(n), c(n), d(n);

	for (uint i = 0; i < n; i++)
		Fast::cin >> a[i] >> b[i] >> c[i] >> d[i];

	std::vector<int> ab, cd;
	ab.reserve(n * n);
	cd.reserve(n * n);

	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++) {
			ab.push_back(a[i] + b[j]);
			cd.push_back(c[i] + d[j]);
		}
	}

	std::ranges::sort(ab);
	std::ranges::sort(cd, std::greater());

	std::vector<std::pair<int, int>> ab_s;
	uint ptr = 0;
	while (ptr < ab.size()) {
		uint i;
		for (i = ptr + 1; i < ab.size(); i++) {
			if (ab[i] != ab[ptr]) break;
		}
		ab_s.emplace_back(ab[ptr], i - ptr);
		ptr = i;
	}

	std::vector<std::pair<int, int>> cd_s;
	ptr = 0;
	while (ptr < cd.size()) {
		uint i;
		for (i = ptr + 1; i < cd.size(); i++) {
			if (cd[i] != cd[ptr]) break;
		}
		cd_s.emplace_back(cd[ptr], i - ptr);
		ptr = i;
	}

	uint l_ptr = 0;
	uint r_ptr = 0;

	int64_t cnt = 0;
	while (l_ptr != ab_s.size() && r_ptr != cd_s.size()) {
		int sum = ab_s[l_ptr].first + cd_s[r_ptr].first;
		if (sum == 0) cnt += (int64_t)ab_s[l_ptr].second * cd_s[r_ptr].second;

		if (l_ptr == ab_s.size() - 1) {
			r_ptr++;
			continue;
		}
		if (r_ptr == cd_s.size() - 1) {
			l_ptr++;
			continue;
		}

		if (sum < 0)
			l_ptr++;
		else
		 	r_ptr++;
	}

	std::cout << cnt;
}
