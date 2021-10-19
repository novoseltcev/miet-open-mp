#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <list>

using namespace std;

int randomize_int(bool nullable = true) {
    random_device rd;
    auto generator = mt19937(rd());
    auto result = (-1) ^ generator() * (!nullable + generator());
    return static_cast<int>(result);
}

vector<int> randomize_array(int size) {
    auto result  = vector<int>();
    for (auto i = 0; i < size; ++i) {
        auto random_value = randomize_int();
        result.push_back(random_value);
    }   return result;
}

template<typename T>
ostream& operator<< (ostream& lhs, const vector<T>& rhs) {
    for (const auto& value: rhs) {
        lhs << value << "\t";
    } return lhs;
}

void setup_num_threads(int size) {
    auto num_threads = omp_get_max_threads();
    omp_set_num_threads( size < num_threads ? size : num_threads );
}

long work(const vector<int>& first_array, const vector<int>& second_array, bool parallel_mode=true) {
    int n = static_cast<int>(first_array.size());
    long result = 0;

    #pragma omp parallel default(none) shared(first_array, second_array, n, result, cout) if (parallel_mode)
    #pragma omp for reduction(+: result)
    for (int i = 0; i < n; i++) {
        long tmp = max(first_array[i] + second_array[i],  4 * first_array[i] - second_array[i]);
        if (tmp > 1)
            result += tmp;
    }
    return result;
}

void time_and_result_tracing(const vector<int>& first_array, const vector<int>& second_array, bool parallel_mode=true) {
    auto size = static_cast<int>(first_array.size());
    setup_num_threads(size);
    cout << "\tCount accessible threads: " << (parallel_mode ? omp_get_max_threads() : 1) << endl;

    auto begin_time = omp_get_wtime();
    auto result = work(first_array, second_array, parallel_mode);
    auto end_time = omp_get_wtime();

    cout << "\tResult: " << result << endl;
    cout << "\tAll working time: " << end_time - begin_time << " c" << endl;
}

int main() {
    auto n = 0;
    cout << "Enter array's size: "; cin >> n;
    auto first_array = randomize_array(n);
    auto second_array = randomize_array(n);
    if (n < 10) {
        cout << "first_array: " << first_array << endl << "second_array: " << second_array << endl;
    }

    cout << "\nParallel mode:\n";
    time_and_result_tracing(first_array, second_array, true);

    cout << "\nSequence mode:\n";
    time_and_result_tracing(first_array, second_array, false);
    return 0;
}
