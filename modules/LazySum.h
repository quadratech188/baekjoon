template <typename T>
class LazySum {
public:
	LazySum():
		_value(), length(1), delta() {}
	LazySum(T value):
		_value(value), length(1), delta() {}
	LazySum(T value, int length):
		_value(value), length(length), delta() {}
private:
	T _value;
	int length;
	T delta;

public:
	T value() const {
		return _value + delta * length;
	}
	operator T() const {
		return value();
	}

	void operator+=(const T& other) {
		delta += other;
	}

	LazySum operator+(const LazySum& other) const {
		return LazySum(value() + other.value(), length + other.length);
	}

	void resolve(LazySum& left, LazySum& right) {
		left += delta;
		right += delta;
		_value = value();
		delta = 0;
	}
};
