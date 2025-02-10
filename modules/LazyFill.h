class LazyFill {
private:
	enum type {
		FILLED,
		UNFILLED,
		MIXED
	};

public:
	LazyFill():
		color(UNFILLED), filled(0), length(1) {}

	LazyFill(type color, int filled, int length):
		color(color), filled(filled), length(length) {}

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
		color = MIXED;
		switch(color) {
			case MIXED:
				return;
			case FILLED:
				left.fill();
				right.fill();
				return;
			case UNFILLED:
				left.unfill();
				right.unfill();
				return;
		}
	}

	LazyFill operator+(const LazyFill& other) const {
		return LazyFill(MIXED, filled + other.filled, length + other.length);
	}
};
