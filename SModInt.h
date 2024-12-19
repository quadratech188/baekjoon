template<typename TYPE, int MOD>
struct SModInt{
	TYPE value;

	inline SModInt() {
		this->value = 0;
	}

	inline SModInt(const TYPE other) {
		this->value = other % MOD;
		if (value < 0) value += MOD;
	}
	
	inline SModInt operator=(const TYPE other) {
		this->value = other % MOD;
		return *this;
	}

	inline SModInt operator+(const SModInt& other) const {
		return {(this->value + other.value) % MOD};
	}

	inline SModInt operator+=(const SModInt& other) {
		this->value = (this->value + other.value) % MOD;
		return *this;
	}

	inline SModInt operator*(const SModInt& other) const {
		return {(this->value * other.value) % MOD};
	}

	inline SModInt operator*(const TYPE other) const {
		return {(this->value * other) % MOD};
	}

	inline SModInt operator*=(const SModInt& other) {
		this->value = (this->value * other.value) % MOD;

		return *this;
	}

	inline SModInt operator-(const SModInt& other) {
		return {(this->value - other.value + MOD) % MOD};
	}

	inline operator int() const {
		return this->value;
	}
};
