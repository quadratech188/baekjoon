#include <istream>

class ModInt {
public:
	ModInt(const int value, const int mod) {
		this->value = value % mod;
		if (this->value < 0) this->value += mod;
		this->mod = mod;
	}

	ModInt(const int mod): value(0), mod(mod) {}

	ModInt(): value(0), mod(0) {}

	ModInt operator+(const int& other) const {
		return ModInt(value + other, mod);
	}

	ModInt& operator++() {
		value = (value + 1) % mod;
		return *this;
	}

	ModInt operator++(int) {
		ModInt temp = *this;
		value = (value + 1) % mod;
		return temp;
	}

	ModInt& operator+=(const ModInt& other) {
		value = (value + other.value) % other.mod;
		mod = other.mod;
		return *this;
	}

	ModInt operator*(const ModInt& other) {
		return ModInt(value * other.value, other.mod);
	}

	void operator=(const int other) {
		value = other % mod;
		if (value < 0) value += mod;
	}

	bool operator<(const int other) {
		return value < other;
	}

	operator int() {
		return value;
	}

private:
	int value;
	int mod;
};

std::istream& operator>>(std::istream& is, ModInt& data) {
	int value;
	is >> value;
	data = value;

	return is;
}
