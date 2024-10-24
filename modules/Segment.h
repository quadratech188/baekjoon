#include <cstddef>

struct Segment {
	size_t start;
	size_t end;
	size_t size() {
		return end - start;
	}
	inline size_t center() {
		return (start + end) / 2;
	}
	inline Segment left() {
		return {start, center()};
	}
	inline Segment right() {
		return {center(), end};
	}
};
