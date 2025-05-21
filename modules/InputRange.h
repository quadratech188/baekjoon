#include <iostream>
#include <iterator>
#include <ranges>

template <typename T, typename Input = std::istream>
inline auto InputRange(size_t n, Input& is = std::cin) {
	return std::views::iota(static_cast<size_t>(0), n)
		| std::views::transform([&is](size_t) {
				T temp;
				std::cin >> temp;
				return temp;
				});
}
