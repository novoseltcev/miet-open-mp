#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <list>
#include <functional>
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

long work(const vector<int>& first_array, const vector<int>& second_array, int begin, int end) {
    long result = 0;
    for (int i = begin; i < end; i++) {
        long tmp = max(first_array[i] + second_array[i],  4 * first_array[i] - second_array[i]);
        if (tmp > 1)
            result += tmp;
    }
    return result;
}

long section_2(const vector<int>& first_array, const vector<int>& second_array) {
    int size = static_cast<int>(first_array.size());
    int chunk = static_cast<int>(size / 2);
    long result = 0;
    #pragma omp parallel default(none) shared(first_array, second_array, result, chunk, size)
    #pragma omp sections reduction(+: result)
    {
        #pragma omp section
        result += work(first_array, second_array, 0, chunk);

        #pragma omp section
        result += work(first_array, second_array, chunk, size);
    }
    return result;
}

long section_4(const vector<int>& first_array, const vector<int>& second_array) {
    int size = static_cast<int>(first_array.size());
    int chunk = static_cast<int>(size / 4);
    long result = 0;
    #pragma omp parallel default(none) shared(first_array, second_array, result, chunk, size)
    #pragma omp sections reduction(+: result)
    {
        #pragma omp section
        result += work(first_array, second_array, 0, chunk);

        #pragma omp section
        result += work(first_array, second_array, chunk, 2 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 2 * chunk, 3 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 3 * chunk, size);
    }
    return result;
}

long section_8(const vector<int>& first_array, const vector<int>& second_array) {
    int size = static_cast<int>(first_array.size());
    int chunk = static_cast<int>(size / 8);
    long result = 0;
    #pragma omp parallel default(none) shared(first_array, second_array, result, chunk, size)
    #pragma omp sections reduction(+: result)
    {
        #pragma omp section
        result += work(first_array, second_array, 0, chunk);

        #pragma omp section
        result += work(first_array, second_array, chunk, 2 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 2 * chunk, 3 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 3 * chunk, 4 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 4 * chunk, 5 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 5 * chunk, 6 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 6 * chunk, 7 * chunk);

        #pragma omp section
        result += work(first_array, second_array, 7 * chunk, size);

    }
    return result;
}

function<long(const vector<int>&, const vector<int>&)> choose_sections(int count) {
    if (count == 2) {
        return section_2;
    } else if (count == 4) {
        return section_4;
    } else if (count == 8) {
        return section_8;
    } else {
        throw error_code();
    }
}

long sections(const vector<int>& first_array, const vector<int>& second_array, int count_sections) {
    return choose_sections(count_sections)(first_array, second_array);
}

double time_tracing(const vector<int>& first_array, const vector<int>& second_array, int count_sections, bool result_tracing=false) {
    auto begin_time = omp_get_wtime();
    auto result = sections(first_array, second_array, count_sections);
    auto end_time = omp_get_wtime();
    if (result_tracing) {
        cout << "\tResult: " << result << endl;
    } return end_time - begin_time;
}

void test_sections(const vector<int>& first_value, const vector<int>& second_value, int count_sections=1) {
    cout << "\nTest " << count_sections << " sections:\n";
    int iteration = static_cast<int>(100000000 / first_value.size());
    double time = 0;
    for (auto i = 0; i < iteration; ++i)
        time += time_tracing(first_value, second_value, count_sections, false);
    cout << "\tWorking time: " << time / iteration << " c\n";
}

void auto_test(int size) {
    cout << "\nSize - " << size;
    auto first_array = randomize_array(size);
    auto second_array = randomize_array(size);

    test_sections(first_array, second_array, 2);
    test_sections(first_array, second_array, 4);
    test_sections(first_array, second_array, 8);
}

int main() {
//    auto n = 0;
//    cout << "Enter array's size: "; cin >> n;
    for (int i = 10; i <= 10000000; i *= 10) {
        auto_test(i);
    }

    return 0;
}
