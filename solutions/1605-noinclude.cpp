#include <cstdint>
#include <cstdio>
#include <iostream>
#include <istream>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T, T MOD>
struct StaticModPolicy {
	static constexpr T mod() {
		return MOD;
	}
};

template <typename T, typename tag = void>
struct DynamicModPolicy {
	static T& mod() {
		static T value = 0;
		return value;
	}
};

template <typename T, typename T2, typename Policy>
class ModInt {
public:

	constexpr ModInt(T2 val) noexcept {
		_val = val % Policy::mod();
		if (_val < 0) _val += Policy::mod();
	}
	constexpr ModInt() noexcept:
		_val(0) {}

	template <typename U>
	constexpr ModInt operator+(U&& other) const noexcept {
		return ModInt(_val + other);
	}

	constexpr ModInt operator+(ModInt const& other) const noexcept {
		return ModInt(_val + other._val);
	}

	constexpr ModInt& operator+=(const ModInt& other) noexcept {
		_val += other._val;
		if (_val >= Policy::mod()) _val -= Policy::mod();
		return *this;
	}

	constexpr ModInt& operator++(int) noexcept {
		_val = (_val + 1) % Policy::mod();
		return *this;
	}

	constexpr ModInt operator-(ModInt const& other) const noexcept {
		return ModInt(Policy::mod() + _val - other._val);
	}

	template <typename U>
	constexpr ModInt operator*(U&& other) const noexcept {
		return ModInt(static_cast<T2>(_val) * other);
	}

	constexpr ModInt operator*(const ModInt& other) const noexcept {
		return ModInt(static_cast<T2>(_val) * other._val);
	}

	constexpr ModInt& operator*=(const ModInt& other) noexcept {
		*this = ModInt(static_cast<T2>(_val) * other._val);
		return *this;
	}

	constexpr ModInt& operator=(const ModInt& other) = default;

	constexpr bool operator==(const ModInt& other) const noexcept {
		return _val == other._val;
	}

	constexpr bool operator!=(const ModInt& other) const noexcept {
		return _val != other._val;
	}

	constexpr bool operator<=(const ModInt& other) const noexcept {
		return _val <= other._val;
	}

	constexpr operator T() const noexcept {
		return _val;
	}

	friend std::ostream& operator<<(std::ostream& os, ModInt const& data) {
		os << data._val;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, ModInt& data) {
		T temp;
		is >> temp;
		data = ModInt(temp);
		return is;
	}

	static void setMod(T mod) {
		Policy::mod() = mod;
	}

private:
	T _val;
};

template <uint32_t MOD>
using sm32 = ModInt<uint32_t, uint64_t, StaticModPolicy<uint32_t, MOD>>;

template <uint64_t MOD>
using sm64 = ModInt<uint64_t, uint64_t, StaticModPolicy<uint64_t, MOD>>;

using sm32_1e9_7 = sm32<1'000'000'007>;
using sm64_1e9_7 = sm64<1'000'000'007>;

template <typename tag = void>
using dm32 = ModInt<uint32_t, uint64_t, DynamicModPolicy<uint32_t, tag>>;

template <typename tag = void>
using dm64 = ModInt<uint64_t, uint64_t, DynamicModPolicy<uint64_t, tag>>;

inline void FastIO() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

sm32_1e9_7 current_hash;

struct custom_hash {
	sm32_1e9_7 operator()(std::string_view view) const {
		return current_hash;
	}
};

int main() {
	FastIO();
	int l;
	std::cin >> l;
	std::string string;
	std::cin >> string;
	std::string_view view(string);
	sm32_1e9_7 p = 29;

	std::vector<sm32_1e9_7> powers(l + 1);

	powers[0] = 1;

	for (int i = 1; i < l + 1; i++) {
		powers[i] = p * powers[i - 1];
	}

	int left = 1;
	int right = l;

	int max = -1;

	while (left <= right) {
		int center = (left + right) / 2;

		std::unordered_set<std::string_view, custom_hash> set;

		bool result = false;

		current_hash = 0;

		for (int i = 0; i < center; i++) {
			current_hash = p * current_hash + string[i];
		}

		set.insert(view.substr(0, center));

		for (int i = 1; i <= l - center; i++) {
			current_hash = p * current_hash + string[i + center - 1] - powers[center] * string[i - 1];

			if (set.contains(view.substr(i, center))) {
				result = true;
				break;
			}
			set.insert(view.substr(i, center));

		}

		if (result) {
			max = center;
			left = center + 1;
		}
		else
			right = center - 1;
	}

	if (max == -1)
		std::cout << '0';
	else
		std::cout << max;
}
