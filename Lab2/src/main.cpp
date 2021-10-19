#include <iostream>
#include <omp.h>
using namespace std;

int main () {
    auto n = 0;
    auto limit = 0;

    cout << "Enter matrix size and parallel limit (n limit):" << endl;
    cin >> n >> limit;

    int i, j;
    float sum;
    float matrix[n][n];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            matrix[i][j] = static_cast<float>(i + j);
        }
    }
    auto start_time = omp_get_wtime();
    #pragma omp parallel default(none) shared(matrix, n, cout) if (n > limit)
    {
        #pragma omp master
        if (omp_get_num_threads() > 1) {
            cout << "Parallel mode!" << endl;
        }

        #pragma omp for private(i,j,sum)
        for (i=0; i < n; i++) {
            sum = 0;
            for (j=0; j < n; j++)
                sum += matrix[i][j];
            if (n < 10) {
                printf ("Сумма элементов строки %d равна %f\n",i,sum);
            }
        }
    } /* Завершение параллельного фрагмента */
    cout << "Working time: " << omp_get_wtime() - start_time << " c" << endl;
    return 0;
}
