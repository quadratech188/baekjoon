template <typename T>
struct no_init {

	T value;

	no_init() {}

	no_init(T value):
		value(value) {}

	T& val() {
		return value;
	}

	no_init& operator=(T&& other) {
		value = other;
		return *this;
	}

	operator T() {
		return value;
	}
};
