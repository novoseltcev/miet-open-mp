#include <iostream>
#include <omp.h>
#include <vector>
#include <random>

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

long work_simple(const vector<int>& first_array, const vector<int>& second_array) {
    int n = static_cast<int>(first_array.size());
    long result = 0;
    for (int i = 0; i < n; i++) {
        long tmp = max(first_array[i] + second_array[i],  4 * first_array[i] - second_array[i]);
        if (tmp > 1)
            result += tmp;
    }
    return result;
}

long work_reduction(const vector<int>& first_array, const vector<int>& second_array) {
    int n = static_cast<int>(first_array.size());
    long result = 0;
    #pragma omp parallel default(none) shared(first_array, second_array, n, result)
    #pragma omp for reduction(+: result)
    for (int i = 0; i < n; i++) {
        long tmp = max(first_array[i] + second_array[i],  4 * first_array[i] - second_array[i]);
        if (tmp > 1)
            result += tmp;
    }
    return result;
}

long work_atomic(const vector<int>& first_array, const vector<int>& second_array) {
    int n = static_cast<int>(first_array.size());
    long result = 0;
    #pragma omp parallel default(none) shared(first_array, second_array, n, result)
    #pragma omp for
    for (int i = 0; i < n; i++) {
        long tmp = max(first_array[i] + second_array[i],  4 * first_array[i] - second_array[i]);
        if (tmp > 1)
            #pragma omp atomic
            result += tmp;
    }
    return result;
}

long work_critical(const vector<int>& first_array, const vector<int>& second_array) {
    int n = static_cast<int>(first_array.size());
    long result = 0;
    #pragma omp parallel default(none) shared(first_array, second_array, n, result)
    #pragma omp for
    for (int i = 0; i < n; i++) {
        long tmp = max(first_array[i] + second_array[i],  4 * first_array[i] - second_array[i]);
        if (tmp > 1)
            #pragma omp critical
            result += tmp;
    }
    return result;
}

double time_and_result_tracing(const vector<int>& first_array, const vector<int>& second_array, long func(const vector<int>&, const vector<int>&)) {
    auto begin_time = omp_get_wtime();
    auto result = func(first_array, second_array);
    auto result_time = omp_get_wtime() - begin_time;
    cout << "\tResult: " << result << endl;
    cout << "\tAll working time: " << result_time << " c" << endl;
    return result_time;
}

int main() {
    auto n = 0;
    cout << "Enter array's size: "; cin >> n;
    auto first_array = randomize_array(n);
    auto second_array = randomize_array(n);


    cout<< "\nAtomic :\n";
    time_and_result_tracing(first_array, second_array, work_atomic);

    cout << "\nCritical:\n";
    time_and_result_tracing(first_array, second_array, work_critical);

    return 0;
}
