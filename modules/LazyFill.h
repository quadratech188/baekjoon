class LazyFill {
private:
	enum type {
		FILLED,
		UNFILLED,
		MIXED
	};

	LazyFill(type color, int filled, int length):
		color(color), filled(filled), length(length) {}

public:
	LazyFill():
		color(UNFILLED), filled(0), length(1) {}

	static LazyFill on() {
		return LazyFill(FILLED, 1, 1);
	}
	static LazyFill off() {
		return LazyFill(UNFILLED, 0, 1);
	}

private:
	type color;
	int filled;
	int length;

public:
	int count() {
		return filled;
	}

	void fill() {
		filled = length;
		color = FILLED;
	}
	void unfill() {
		filled = 0;
		color = UNFILLED;
	}

	void resolve(LazyFill& left, LazyFill& right) {
		switch(color) {
			case MIXED:
				return;
			case FILLED:
				left.fill();
				right.fill();
				break;
			case UNFILLED:
				left.unfill();
				right.unfill();
				break;
		}
		color = MIXED;
	}

	LazyFill operator+(const LazyFill& other) const {
		return LazyFill(MIXED, filled + other.filled, length + other.length);
	}
};
