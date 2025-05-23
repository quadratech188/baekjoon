#include <iterator>
#include <sys/types.h>

template <typename T, typename Length = std::size_t>
class LazySum {
public:
	// Builder
	template <typename value>
	using with_length = LazySum<T, value>;

private:
	using size_t = Length;

public:
	using extracted_t = T;

	LazySum():
		value(), length(1), delta() {}
	LazySum(T&& value):
		value(value), length(1), delta() {}
	LazySum(T&& value, size_t length):
		value(value), length(length), delta() {}

private:
	T value;
	size_t length;
	T delta;

public:
	T extract() const {
		return value + delta * length;
	}

	void operator+=(const T& other) {
		delta += other;
	}

	LazySum operator+(const LazySum& other) const {
		return LazySum(extract() + other.extract(), length + other.length);
	}

	void propagate(LazySum& left, LazySum& right) {
		left += delta;
		right += delta;
	}

	void reinit(LazySum const& left, LazySum const& right) {
		value = left.extract() + right.extract();
		delta = 0;
	}

	void apply() {
		value = extract();
		delta = 0;
	}
};
