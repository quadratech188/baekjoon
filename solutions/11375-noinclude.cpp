#include <ranges>
#include <unistd.h>
#include <vector>

#ifndef FASTISTREAM_BUFFER_SIZE
#define FASTISTREAM_BUFFER_SIZE 1 << 20
#endif

#ifndef FASTOSTREAM_BUFFER_SIZE
#define FASTOSTREAM_BUFFER_SIZE 1 << 20
#endif

namespace Fast {
	class istream {
	private:
		inline char getchar() {
			static char buffer[FASTISTREAM_BUFFER_SIZE];
			static char* ptr = buffer;
			static char* end = buffer;

			if (ptr == end) {
				ssize_t size = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (size <= 0) return EOF;
				ptr = buffer;
				end = buffer + size;
			}
			return *(ptr++);
		}
	public:
		template <typename T>
		inline istream& operator>>(T& val)
		requires std::is_integral_v<T> {
			char ch;
			val = 0;

			do {
				ch = getchar();
			} while (std::isspace(ch));

			// Optimized away for non-signed types
			bool negative = false;
			if constexpr (std::is_signed_v<T>) {
				if (ch == '-') {
					negative = true;
					ch = getchar();
				}
			}

			do {
				val = 10 * val + ch - '0';
				ch = getchar();
			} while ('0' <= ch && ch <= '9');

			if constexpr (std::is_signed_v<T>)
				if (negative) val = -val;

			return *this;
		}

		inline istream& operator>>(char& val) {
			do {
				val = getchar();
			} while (std::isspace(val));
			return *this;
		}

		template <typename T1, typename T2>
		inline istream& operator>>(std::pair<T1, T2>& pair) {
			(*this) >> pair.first >> pair.second;
			return *this;
		}

		template <typename... Ts>
		inline istream& operator>>(std::tuple<Ts...>& tuple){
			std::apply([this](auto&... x) {(((*this) >> x), ...);}, tuple);
			return *this;
		}

		template <typename T>
		std::vector<T> to_vec(uint size) {
			std::vector<T> result(size);

			for (auto& val: result)
				(*this) >> val;

			return result;
		}

		template <typename T>
		auto to_range(uint size) {
			return std::views::iota(0u, size)
				| std::views::transform([this](uint) {
						T temp;
						(*this) >> temp;
						return temp;
						});
		}
	};

	istream cin;
}
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

std::vector<bool> visited;
std::vector<std::vector<uint>> potential_jobs;
std::vector<uint> assigned_workers;

bool reassign(uint worker) {
	for (uint job: potential_jobs[worker]) {
		if (visited[job]) continue;

		visited[job] = true;

		if (assigned_workers[job] == std::numeric_limits<uint>::max() || reassign(assigned_workers[job])) {
			assigned_workers[job] = worker;
			return true;
		}
	}
	return false;
}

int main() {
	uint n, m;
	Fast::cin >> n >> m;

	visited.resize(m);
	potential_jobs.resize(n);
	assigned_workers.resize(m, std::numeric_limits<uint>::max());

	for (auto& job_list: potential_jobs) {
		uint cnt;
		Fast::cin >> cnt;
		job_list = Fast::cin.to_vec<uint>(cnt);
		for (auto& x: job_list)
			x --;
	}

	int result = 0;
	for (uint i = 0; i < n; i++) {
		visited.assign(m, false);
		if (reassign(i)) result ++;
	}
	std::cout << result;
}
