#include <numeric>

template <typename T>
class frac {
public:
	frac():
		_top(1), _bottom(1) {}

	frac(T value):
		_top(value), _bottom(1) {}

	frac(T top, T bottom) {
		T gcd = std::gcd(top, bottom);

		_top = top / gcd;
		_bottom = bottom / gcd;
	}

private:
	struct raw {};
	frac(T top, T bottom, raw):
		_top(top), _bottom(bottom) {}

	T _top;
	T _bottom;

public:
	static frac verified(T top, T bottom) {
		return frac(top, bottom, {});
	}

	frac operator*(frac const& other) const {
		return frac(_top * other._top, _bottom * other._bottom);
	}

	T& bottom() {
		return _bottom;
	}
	T& top() {
		return _top;
	}

	T to_T(T const& multiplier) {
		return _top * (multiplier / _bottom);
	}
};
