#include <cassert>
#include <vector>
template <typename T>
class CircularQueue {
public:
	CircularQueue(size_t n):
		data(n), front_ptr(0), back_ptr(0), n(n) {}
private:
	size_t front_ptr, back_ptr;
	const size_t n;
	std::vector<T> data;
public:
	bool empty() const noexcept {
		return front_ptr == back_ptr;
	}

	T& front() {
		return data[front_ptr];
	}

	const T& front() const {
		return data[front_ptr];
	}

	void pop() {
		front_ptr = (front_ptr + 1) % n;
	}

	void push(const T& value) {
		data[back_ptr] = value;
		back_ptr = (back_ptr + 1) % n;
	}

	void push(T&& value) {
		data[back_ptr] = std::move(value);
		back_ptr = (back_ptr + 1) % n;
	}
};
