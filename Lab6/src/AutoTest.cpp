//
// Created by st0a0novoseltcev on 9/22/21.
//

#include "AutoTest.h"


double AutoTest::timeTracing(const function<long(const vector<int>&, const vector<int>&)>& func, bool result_tracing=false) const {
    auto begin_time = omp_get_wtime();
    auto result = func(firstArray, secondArray);
    if (result_tracing)
        cout << "\tResult: " << result << endl;
    return omp_get_wtime() - begin_time;
}

AutoTest::AutoTest(int size): size(size) {
    firstArray  = randomizeArray(size);
    secondArray = randomizeArray(size);
}

double AutoTest::run(int iteration, const function<long(const vector<int>&, const vector<int>&)>& func, bool result_tracing) const {
    double time = 0;
    for (auto i = 0; i < iteration; ++i)
        time += timeTracing(func);
    if (result_tracing)
        timeTracing(func, true);
    return time / iteration;
}
