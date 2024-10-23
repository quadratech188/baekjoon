#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <utility>

using namespace std;

bool is_prime(int n) {
    if (n <= 1) return false;
    int sqrt_n = sqrt(n);
    for (int i = 2; i <= sqrt_n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

long long solution(int current_index, int sum, 
                  const vector<pair<int, int>>& values,
                  unordered_map<string, long long>& cache) {
    if (current_index == values.size()) {
        return is_prime(sum) ? 1 : 0;
    }

    string cache_key = to_string(current_index) + "," + to_string(sum);
    if (cache.count(cache_key)) {
        return cache[cache_key];
    }

    long long result = 0;
    for (int times_used = 0; times_used <= values[current_index].second; times_used++) {
        result += solution(current_index + 1, 
                         sum + values[current_index].first * times_used,
                         values, cache);
    }

    cache[cache_key] = result;
    return result;
}

int main() {
    int n;
    cin >> n;

    unordered_map<int, int> value_dict;
    for (int i = 0; i < n; i++) {
        int value;
        cin >> value;
        value_dict[value]++;
    }

    vector<pair<int, int>> values;
    for (const auto& pair : value_dict) {
        values.emplace_back(pair.first, pair.second);
    }

    unordered_map<string, long long> cache;

    cout << solution(0, 0, values, cache) << endl;

    return 0;
}
