#include "FastIO2.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

std::vector<bool> visited;
std::vector<std::vector<uint>> potential_jobs;
std::vector<uint> assigned_workers;

bool reassign(uint worker) {
	for (uint job: potential_jobs[worker]) {
		if (visited[job]) continue;

		visited[job] = true;

		if (assigned_workers[job] == std::numeric_limits<uint>::max() || reassign(assigned_workers[job])) {
			assigned_workers[job] = worker;
			return true;
		}
	}
	return false;
}

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	visited.resize(m);
	potential_jobs.resize(n);
	assigned_workers.resize(m, std::numeric_limits<uint>::max());

	for (auto& job_list: potential_jobs) {
		uint cnt;
		Fast::cin >> cnt;
		job_list = Fast::cin.to_vec<uint>(cnt);
		for (auto& x: job_list)
			x --;
	}

	int result = 0;
	for (uint i = 0; i < n; i++) {
		visited.assign(m, false);
		if (reassign(i)) result ++;
	}
	std::cout << result;
}
