//#ifndef LAB6_AUTOTEST_H
//#define LAB6_AUTOTEST_H
#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <list>
#include <functional>

using namespace std;


class AutoTest {
private:
    vector<int> firstArray;
    vector<int> secondArray;

    static int randomizeInt(bool nullable = true) {
        random_device rd;
        auto generator = mt19937(rd());
        auto result = (-1) ^ generator() * (!nullable + generator());
        return static_cast<int>(result);
    }

    static vector<int> randomizeArray(int size) {
        auto result  = vector<int>();
        for (auto i = 0; i < size; ++i) {
            auto random_value = randomizeInt();
            result.push_back(random_value);
        }   return result;
    }


    double timeTracing(const function<long(const vector<int>&, const vector<int>&)>&, bool) const;

public:
    uint size;
    AutoTest(int);

    double run (int, const function<long(const vector<int>&, const vector<int>&)>&, bool result_tracing = false) const;

};


//#endif //LAB6_AUTOTEST_H
