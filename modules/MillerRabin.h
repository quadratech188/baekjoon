#include <array>

#include "ModInt.h"
#include "Math.h"

namespace Math {
template <typename T>
	bool prime(T p) {
		T d = p - 1;
		int s = 0;

		while (d % 2 == 0) {
			s ++;
			d /= 2;
		}

		static std::array<int, 12> bases = {
			2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
		};

		ModInt<T>::setMod(p);


		for (T a: bases) {
			if (a >= p - 1) break;

			ModInt<T> x = power(ModInt<T>(a), d);
			ModInt<T> y = 0;

			for (int i = 0; i < s; i++) {
				y = x * x;
				if (y == 1 && x != 1 && x != p - 1) return false;

				x = y;
			}
			if (y != 1) return false;
		}

		return true;
	}
}
