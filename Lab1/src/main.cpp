#include <iostream>
#include <omp.h>
#include <list>
#include <random>
using namespace std;

int randomize_int(bool nullable = false) {
    random_device rd;
    auto generator = mt19937(rd());
    auto result = (-1) ^ generator() * (!nullable + generator());
    return static_cast<int>(result);
}

list<list<int>> randMatrix(int row_size, int col_size) {
    auto result  = list<list<int>>();
    for (auto i = 0; i < row_size; ++i) {
        auto tmp_list = list<int>();
        for (int j = 0; j < col_size; ++j) {
            auto random_value = randomize_int();
            tmp_list.push_back(random_value);
        } result.push_back(tmp_list);
    }   return result;
}

template<typename T>
ostream& operator<< (ostream& lhs, const list<T>& rhs) {
    for (const auto& value: rhs) {
        lhs << value << "\t";
    } return lhs;
}

template<typename T>
void printMatrix(list<list<T>> matrix) {
    for (const auto& row: matrix) {
        cout << row << endl;
    }
}

int count_seven_digit_in_pairs_sum_by_rows(const list<list<int>>& matrix, bool parallel_mode=true) {
    if (!parallel_mode) {
        omp_set_num_threads(1);
    }
    auto num_threads = omp_get_max_threads();
    if (matrix.size() < num_threads) {
        omp_set_num_threads(static_cast<int>(matrix.size()));
        num_threads = omp_get_max_threads();
    }

    cout << "\tCount accessible threads: " << num_threads << endl;
    int buffer[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        buffer[i] = 0;
    }

    #pragma omp parallel default(none) shared(matrix, buffer)
    {
        #pragma omp for schedule(dynamic)
        for (auto i = 0; i < matrix.size(); i++) {
            auto current_ptr = matrix.begin();
            advance(current_ptr, i);
            auto row = *current_ptr;
            auto sum = 0;
            for (const auto& first_val: row) {
                for (const auto &second_val: row) {
                    auto pair_sum = first_val + second_val;
                    while (pair_sum != 0) {
                        if (abs(pair_sum % 10) == 7) {
                            sum++;
                        } pair_sum /= 10;
                    }
                }
            }
            buffer[omp_get_thread_num()] += sum;
        }
    }

    auto result = 0;
    for (int i = 0; i < num_threads; ++i) {
        result += buffer[i];
    } return result;
}

void time_and_result_tracing(const list<list<int>>& matrix, bool parallel_mode=true) {
    auto begin_time = omp_get_wtime();
    auto result = count_seven_digit_in_pairs_sum_by_rows(matrix, parallel_mode);
    auto end_time = omp_get_wtime();
    cout << "\tResult: " << result << endl;
    cout << "\tAll working time: " << end_time - begin_time << " c" << endl;
}

int main() {
    int N, M;
    cout << "Enter size (N M)" << endl;
    cin >> N >> M;
    auto testMatrix = randMatrix(N, M);
    cout << "size: " << N << "x" << M << endl;
    if (N < 10 and M < 10) {
        printMatrix(testMatrix);
    }

    cout << "\nIn parallel mode:\n";
    time_and_result_tracing(testMatrix);

    cout << "\nIn sequence mode:\n";
    time_and_result_tracing(testMatrix, false);

    auto x = 0;
    cin >> x;

    return 0;
}