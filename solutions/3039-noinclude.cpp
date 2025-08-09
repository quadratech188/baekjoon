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
#include <vector>
#include <algorithm>
#include <unordered_map>

template <typename T>
class Compress {
public:
	Compress() {}

	template <std::ranges::range R>
	Compress(R&& values) {
		size_t index = 0;

		for (T& value: values) {
			if (_inverse.find(value) != _inverse.end()) continue;
			_inverse[value] = index;
			index++;
		}

		_size = index;

		_values.resize(_size);

		for (auto& it: _inverse) {
			_values[it.second] = it.first;
		}
	}

	size_t compress(const T& val) {
		return _inverse.at(val);
	}

	T decompress(const size_t index) {
		return _values[index];
	}

	size_t size() const {
		return _size;
	}

private:
	size_t _size;
	std::vector<T> _values;
	std::unordered_map<T, size_t> _inverse;
};
#include <algorithm>

template <typename T>
class Min {
public:
	T operator()(const T& l, const T& r) const {
		return std::min(l, r);
	}
};

template <typename T>
class Max {
public:
	T operator()(const T& l, const T& r) const {
		return std::max(l, r);
	}
};

template <typename T>
class Mul {
public:
	T operator()(const T& l, const T& r) const {
		return l * r;
	}
};

template <typename T, typename Operator>
class OverloadedPlus {
public:
	using value_type = T;

	OverloadedPlus() {}

	OverloadedPlus(T const& value, Operator op = Operator()):
		value(value), op(op) {}
private:
	T value;
	[[no_unique_address]]
	Operator op;
public:
	explicit operator T const&() {
		return value;
	}

	T const& val() {
		return value;
	}

	OverloadedPlus operator+(OverloadedPlus const& other) {
		return op(value, other.value);
	}

	OverloadedPlus operator+(T const& other) {
		return op(value, other);
	}

	void reinit(OverloadedPlus const& l, OverloadedPlus const& r) {
		value = op(l.value, r.value);
	}
};

template <typename T>
using maxxer = OverloadedPlus<T, Max<T>>;

template <typename T>
using minner = OverloadedPlus<T, Min<T>>;
#include <iostream>
#include <limits>
#include <set>
#include <variant>

struct Query {
	char type;
	int a, b;
	uint i;
};

int main() {
	uint n;
	Fast::cin >> n;

	std::vector<std::pair<int, int>> additions;
	std::vector<Query> queries(n);

	std::vector<int> knowledges;

	uint index = 0;
	for (auto& query: queries) {
		Fast::cin >> query.type;
		if (query.type == 'D') {
			Fast::cin >> query.a >> query.b;
			query.i = index;
			index ++;
			additions.emplace_back(query.a, query.b);
			knowledges.push_back(query.b);
		}
		else
		 	Fast::cin >> query.i;
	}

	std::ranges::sort(knowledges);
	Compress<int> knowledge_compress(knowledges);

	auto set_compare = [&additions](uint l, uint r) {
		return additions[l].first < additions[r].first;
	};

	std::vector<std::set<uint, decltype(set_compare)>> students;

	for (size_t i = 0; i < knowledge_compress.size(); i++)
		students.emplace_back(set_compare);

	SegmentTree<maxxer<int>> max_tree(knowledge_compress.size(), std::numeric_limits<int>::min());

	for (auto& query: queries) {
		if (query.type == 'D') {
			int temp = query.a;

			uint knowledge_index = knowledge_compress.compress(query.b);

			max_tree.update(knowledge_index, [temp](maxxer<int>& x) {x = x + temp;});
			students[knowledge_index].insert(query.i);
			continue;
		}

		auto [understanding, knowledge] = additions[query.i - 1];

		uint start = knowledge_compress.compress(knowledge);

		auto at_start = students[start].upper_bound(query.i - 1);
		if (at_start != students[start].end()) {
			std::cout << *at_start + 1 << '\n';
			continue;
		}

		uint l_ptr = start + 1;
		uint r_ptr = max_tree.size() - 1;

		if (l_ptr > r_ptr) {
			std::cout << "NE\n";
			continue;
		}

		while (l_ptr != r_ptr) {
			uint center = (l_ptr + r_ptr) / 2;
			int val = max_tree.sum(start + 1, center + 1).val();
			
			if (val >= understanding)
				r_ptr = center;
			else
				l_ptr = center + 1;
		}

		auto answer = students[l_ptr].lower_bound(query.i - 1);
		if (answer == students[l_ptr].end())
			std::cout << "NE\n";
		else
		 	std::cout << *answer + 1 << '\n';
	}
}
