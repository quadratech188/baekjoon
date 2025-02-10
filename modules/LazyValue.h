#include <functional>

template <typename T, typename Operator = std::plus<T>, T initial = T()>
class LazyValue {
public:
	LazyValue():
		_value(initial), delta(initial), op() {}
	LazyValue(T value):
		_value(value), delta(initial), op() {}

private:
	T _value;
	T delta;
	Operator op;

public:
	T value() const {
		return op(_value, delta);
	}

	void operator+=(const T& other) {
		delta = op(delta, other);
	}

	LazyValue operator+(const LazyValue& other) const {
		return LazyValue(op(value(), other.value()));
	}

	void resolve(LazyValue& left, LazyValue& right) {
		left.delta = op(left.delta, delta);
		right.delta = op(right.delta, delta);

		_value = value();
		delta = T();
	}
};
