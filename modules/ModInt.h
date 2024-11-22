template<typename TYPE, int MOD>
struct ModInt{
	TYPE value;

	inline ModInt() {
		this->value = 0;
	}

	inline ModInt(const TYPE other) {
		this->value = other % MOD;
		if (value < 0) value += MOD;
	}
	
	inline ModInt operator=(const TYPE other) {
		this->value = other % MOD;
		return *this;
	}

	inline ModInt operator+(const ModInt& other) const {
		return {(this->value + other.value) % MOD};
	}

	inline ModInt operator+=(const ModInt& other) {
		this->value = (this->value + other.value) % MOD;
		return *this;
	}

	inline ModInt operator*(const ModInt& other) const {
		return {(this->value * other.value) % MOD};
	}

	inline ModInt operator*(const TYPE other) const {
		return {(this->value * other) % MOD};
	}

	inline ModInt operator*=(const ModInt& other) {
		this->value = (this->value * other.value) % MOD;

		return *this;
	}

	inline ModInt operator-(const ModInt& other) {
		return {(this->value - other.value + MOD) % MOD};
	}

	inline operator int() const {
		return this->value;
	}
};
