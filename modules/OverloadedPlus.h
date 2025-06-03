#include "Operators.h"

template <typename T, typename Operator>
class OverloadedPlus {
public:
	using value_type = T;

	OverloadedPlus() {}

	OverloadedPlus(T const& value, Operator op = Operator()):
		value(value), op(op) {}
private:
	T value;
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
	void reinit(OverloadedPlus const& l, OverloadedPlus const& r) {
		value = op(l, r);
	}
};

template <typename T>
using maxxer = OverloadedPlus<T, Max<T>>;

template <typename T>
using minner = OverloadedPlus<T, Min<T>>;
