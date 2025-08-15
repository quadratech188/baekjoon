#include <ranges>
#include <unistd.h>
#include <vector>

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}

		template <typename T1, typename T2>
		inline istream& operator>>(std::pair<T1, T2>& pair) {
			(*this) >> pair.first >> pair.second;
			return *this;
		}

		template <typename... Ts>
		std::tuple<Ts...> get() {
			std::tuple<Ts...> result;

			std::apply([this](auto&... x) {(((*this) >> x), ...);}, result);

			return result;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}

		template <typename T>
		auto to_range(uint size) {
			return std::views::iota(0u, size)
				| std::views::transform([this](uint) {
						T temp;
						(*this) >> temp;
						return temp;
						});
		}
	};

	istream cin;
}
#include <concepts>
#include <functional>
#include <ranges>

#include <cstddef>

struct Segment {
	constexpr Segment(): start(0), end(0) {}
	constexpr Segment(size_t start, size_t end): start(start), end(end) {}

	size_t start;
	size_t end;
	constexpr size_t size() const {
		return end - start;
	}
	constexpr size_t center() const {
		return (start + end) / 2;
	}
	constexpr Segment left() const {
		return Segment(start, center());
	}
	constexpr Segment right() const {
		return Segment(center(), end);
	}

	constexpr bool includes(const Segment& other) const {
		return start <= other.start && other.end <= end;
	}
};
template <typename T>
class DummyIterator {
public:
	DummyIterator(const T& val): _val(val) {}

	const T& operator*() const {
		return _val;
	}

	DummyIterator& operator++() {
		return *this;
	}
	void operator++(int) {}

private:
	const T& _val;
};

template <typename T>
class SegmentTree {
public:
	using value_type = T;

	template <std::ranges::range R>
	SegmentTree(R&& range):
		_size(std::ranges::size(range)), _values(4 * _size) {
		auto it = std::ranges::begin(range);
		init(Segment(0, _size), 0, it);
	}

	SegmentTree(size_t size, T&& default_value = T()):
		_size(size), _values(4 * size) {
		auto it = DummyIterator<T>(default_value);
		init(Segment(0, _size), 0, it);
	}

	template <typename Extractor = std::identity>
	auto sum(Segment segment, Extractor extractor = {}) {
		return sum(segment, Segment(0, _size), 0, extractor);
	}

	template <typename Extractor = std::identity>
	auto sum(size_t start, size_t end, Extractor extractor = {}) {
		return sum(Segment(start, end), extractor);
	}

	template <typename Callable>
	void update(size_t index, Callable func) {
		return update(index, 0, Segment(0, _size), func);
	}

	constexpr size_t size() const noexcept {
		return _size;
	}

	constexpr T const& root() const noexcept {
		return _values[0];
	}

private:
	size_t const _size;
	std::vector<T> _values;

	template <typename Iter>
	void init(Segment segment, size_t index, Iter& iterator) {
		if (segment.size() == 1) {
			_values[index] = T(*iterator);
			++iterator;
			return;
		}

		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;

		init(segment.left(), left, iterator);
		init(segment.right(), right, iterator);

		_values[index] = _values[left] + _values[right];
	}

	template <typename Extractor>
	auto sum(Segment const query, Segment const segment, size_t const index, Extractor extractor) {
		if (query.includes(segment))
			return std::invoke(extractor, static_cast<T const&>(_values[index]));

		size_t const left = index * 2 + 1;
		size_t const right = index * 2 + 2;

		if (segment.center() <= query.start)
			return sum(query, segment.right(), right, extractor);

		if (query.end <= segment.center())
			return sum(query, segment.left(), left, extractor);

		return sum(query, segment.left(), left, extractor)
		     + sum(query, segment.right(), right, extractor);
	}

	template <typename Callable>
	void update(size_t const index, size_t const value_index, Segment const segment, Callable func) {
		if (segment.size() == 1) {
			std::invoke(func, _values[value_index]);
			return;
		}

		size_t const left = value_index * 2 + 1;
		size_t const right = value_index * 2 + 2;

		if (index < segment.center())
			update(index, left, segment.left(), func);
		else
			update(index, right, segment.right(), func);

		if constexpr (requires (T x, T const& y, T const& z) {x.reinit(y, z);})
			_values[value_index].reinit(_values[left], _values[right]);
		else
		 	_values[value_index] = _values[left] + _values[right];
	}
};
#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

struct Data {
	uint32_t all;
	uint32_t any;

	Data(uint32_t algorithm_set = 0):
		all(algorithm_set), any(algorithm_set) {}
	Data(uint32_t all, uint32_t any):
		all(all), any(any) {}

	Data operator+(Data const& other) const {
		return {all & other.all, any | other.any};
	}
};
struct Student {
	int skill;
	uint32_t algorithms;

	bool operator<(Student const& other) const {
		return skill < other.skill;
	}

	template <typename IS>
	friend IS& operator>> (IS& is, Student& student) {
		uint m;
		is >> m >> student.skill;
		student.algorithms = 0;
		for (uint i = 0; i < m; i++) {
			uint a;
			is >> a;
			student.algorithms += (1 << a);
		}
		return is;
	}
};

int main() {
	uint n, k;
	int d;
	Fast::cin >> n >> k >> d;

	auto students = Fast::cin.to_vec<Student>(n);

	std::sort(students.begin(), students.end());

	SegmentTree<Data> tree(students | std::views::transform([](Student const& s) {
				return s.algorithms;
				}));
	uint l = 0;
	uint r = 1;

	int max = 0;

	while (true) {
		int skill_diff = students[r - 1].skill - students[l].skill;
		if (skill_diff <= d) {
			Data result = tree.sum(l, r);
			max = std::max(max, (std::popcount(result.any) - std::popcount(result.all)) * int(r - l));
		}

		if (skill_diff <= d || r - l == 1) {
			if (r == n) break;
			r ++;
		}
		else
			l ++;
	}
	std::cout << max;
}
