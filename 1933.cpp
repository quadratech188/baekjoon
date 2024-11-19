#include "modules/LazySegmentTree.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

struct Building {
	int left;
	int right;
	int height;
};

int main() {
	std::unordered_set<int> pointsOfInterest;
	std::vector<Building> buildings;

	int n;
	std::cin >> n;
	
	for (int i = 0; i < n; i++) {
		int l, h, r;
		buildings.push_back({l, r, h});
		pointsOfInterest.push_back(l);
		pointsOfInterest.push_back(r);
	}

	std::sort(pointsOfInterest.begin(), pointsOfInterest.end());

	std::unordered_map<int, int> lookup;

	for (int i = 0; i < pointsOfInterest.size(); i++) {
		lookup[pointsOfInterest.]
	}
}
