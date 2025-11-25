#include <queue>

namespace std {
	template <typename T>
	using minority_queue = priority_queue<T, std::vector<T>, std::greater<T>>;
}

template <typename OS, std::ranges::range R>
OS& operator<<(OS& os, R const& range) {
	for (auto const& x: range) {
		os << x << ' ';
	}
	return os;
}
