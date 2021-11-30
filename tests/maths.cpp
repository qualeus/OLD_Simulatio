#include "test.hpp"

template <class T>
using Unit = T;

template <class T>
using Vect = std::vector<T>;

template <class T>
using Pair = std::vector<std::pair<T, T>>;

template <class T>
using SVect = std::vector<std::shared_ptr<T>>;

template <class T>
using SPair = std::vector<std::pair<std::shared_ptr<T>, std::shared_ptr<T>>>;

template <class T>
std::pair<SVect<T>, SPair<T>> CreatePairs(std::vector<T> values) {
    SVect<T> vect = SVect<T>();
    SPair<T> pair = SPair<T>();

    for (int i = 0; i < values.size(); i++) {
        vect.push_back(std::make_shared<T>(values.at(i)));
        for (int j = 0; j < i; j++) { pair.push_back({std::make_shared<T>(values.at(j)), std::make_shared<T>(values.at(i))}); }
    }
    return std::pair<SVect<T>, SPair<T>>({vect, pair});
}

template <class T>
Pair<T> ExtractPair(SPair<T> spair) {
    Pair<T> pair = Pair<T>();
    for (int i = 0; i < spair.size(); i++) { pair.push_back({*spair.at(i).first, *spair.at(i).second}); }
    return pair;
}

template <class T>
Vect<T> ExtractVect(SVect<T> svect) {
    Vect<T> vect = Vect<T>();
    for (int i = 0; i < svect.size(); i++) { vect.push_back(*svect.at(i)); }
    return vect;
}

template <class T>
void RemovePairs(int index_delete, std::vector<T> values) {
    /*
    Ex: We remove the 3:

    Step0 [3]: {0,1},{0,2},{1,2},{0,3},{1,3},{2,3},{0,4},{1,4},{2,4},{3,4},{0,5},{1,5},{2,5},{3,5} ...
    Step1 (.): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{3,4},{0,5},{1,5},{2,5},{3,5},{4,5},{0,6},{1,6}  ...
    Step2 (6): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{0,5},{1,5},{2,5},{3,5},{4,5},{0,6},{1,6},{2,6}  ...
    Step3 (9): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{0,5},{1,5},{2,5},{4,5},{0,6},{1,6},{2,6},{3,6}  ...
    Step4 (13): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{0,5},{1,5},{2,5},{4,5},{0,6},{1,6},{2,6},{4,6}  ...
    Step5 (18):  [5] ... {0,5},{1,5},{2,5},{4,5},{0,6},{1,6},{2,6},{4,6},{5,6},{0,7},{1,7},{2,7}  ...
    Step6 (24):  [11] ... {2,6},{4,6},{5,6},{0,7},{1,7},{2,7},{4,7},{5,7},{6,7},{0,8},{1,8},{2,8}  ...
    */
    std::cout << "\nInput Vector: " << gmt::to_string(values) << std::endl;

    std::pair<SVect<int>, SPair<int>> test_result = CreatePairs(values);

    gmt::remove_unordered(index_delete, test_result.first);
    gmt::remove_pairs(index_delete, test_result.second);
    std::cout << "\nRemoving Pair [" << gmt::to_string(index_delete) << "]... => test_result" << std::endl;

    values.erase(values.begin() + index_delete);
    std::cout << "Manual Remove [" << gmt::to_string(index_delete) << "]... => true_result" << std::endl;

    Vect<int> test_result_vect = ExtractVect(test_result.first);
    Pair<int> test_result_pair = ExtractPair(test_result.second);

    std::cout << "\ntest_result_vect: " << gmt::to_string(test_result_vect) << std::endl;
    std::cout << "test_result_pair: " << gmt::to_string(test_result_pair) << std::endl;

    std::pair<SVect<int>, SPair<int>> true_result = CreatePairs(values);

    Vect<int> true_result_vect = ExtractVect(true_result.first);
    Pair<int> true_result_pair = ExtractPair(true_result.second);

    std::cout << "\ntrue_result_vect: " << gmt::to_string(true_result_vect) << std::endl;
    std::cout << "true_result_pair: " << gmt::to_string(true_result_pair) << std::endl;

    ASSERT_EQUAL(true_result_vect.size(), test_result_vect.size());
    for (int i = 0; i < test_result_vect.size(); i++) { ASSERT_EQUAL(true_result_vect.at(i), test_result_vect.at(i)); }

    ASSERT_EQUAL(true_result_pair.size(), test_result_pair.size());
    for (int i = 0; i < test_result_pair.size(); i++) { ASSERT_EQUAL(true_result_pair.at(i), test_result_pair.at(i)); }
}

template <class T>
void RemovePairsUnordered(int index_delete, std::vector<T> values) {
    /*
    Ex: We remove the 3:

    Step0 [3]: {0,1},{0,2},{1,2},{0,3},{1,3},{2,3},{0,4},{1,4},{2,4},{3,4},{0,5},{1,5},{2,5},{3,5} ...
    Step1 (.): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{3,4},{0,5},{1,5},{2,5},{3,5},{4,5},{0,6},{1,6}  ...
    Step2 (6): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{0,5},{1,5},{2,5},{3,5},{4,5},{0,6},{1,6},{2,6}  ...
    Step3 (9): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{0,5},{1,5},{2,5},{4,5},{0,6},{1,6},{2,6},{3,6}  ...
    Step4 (13): {0,1},{0,2},{1,2},{0,4},{1,4},{2,4},{0,5},{1,5},{2,5},{4,5},{0,6},{1,6},{2,6},{4,6}  ...
    Step5 (18):  [5] ... {0,5},{1,5},{2,5},{4,5},{0,6},{1,6},{2,6},{4,6},{5,6},{0,7},{1,7},{2,7}  ...
    Step6 (24):  [11] ... {2,6},{4,6},{5,6},{0,7},{1,7},{2,7},{4,7},{5,7},{6,7},{0,8},{1,8},{2,8}  ...
    */
    std::cout << "\nInput Vector: " << gmt::to_string(values) << std::endl;

    std::pair<SVect<int>, SPair<int>> test_result = CreatePairs(values);

    auto ptr = gmt::remove_unordered_return(index_delete, test_result.first);
    gmt::remove_pairs_unordered(ptr, test_result.second);
    std::cout << "\nRemoving Pair [" << gmt::to_string(index_delete) << "]... => test_result" << std::endl;

    values.erase(values.begin() + index_delete);
    std::cout << "Manual Remove [" << gmt::to_string(index_delete) << "]... => true_result" << std::endl;

    Vect<int> test_result_vect = ExtractVect(test_result.first);
    Pair<int> test_result_pair = ExtractPair(test_result.second);

    std::cout << "\ntest_result_vect: " << gmt::to_string(test_result_vect) << std::endl;
    std::cout << "test_result_pair: " << gmt::to_string(test_result_pair) << std::endl;

    std::pair<SVect<int>, SPair<int>> true_result = CreatePairs(values);

    Vect<int> true_result_vect = ExtractVect(true_result.first);
    Pair<int> true_result_pair = ExtractPair(true_result.second);

    std::cout << "\ntrue_result_vect: " << gmt::to_string(true_result_vect) << std::endl;
    std::cout << "true_result_pair: " << gmt::to_string(true_result_pair) << std::endl;

    ASSERT_EQUAL(true_result_vect.size(), test_result_vect.size());
    for (int i = 0; i < test_result_vect.size(); i++) { ASSERT_EQUAL(true_result_vect.at(i), test_result_vect.at(i)); }

    ASSERT_EQUAL(true_result_pair.size(), test_result_pair.size());
    for (int i = 0; i < test_result_pair.size(); i++) { ASSERT_EQUAL(true_result_pair.at(i), test_result_pair.at(i)); }
}

int main(int, char**) {
    std::vector<int> values = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    RemovePairs(0, values);
    RemovePairs(1, values);
    RemovePairs(2, values);
    RemovePairs(3, values);
    RemovePairs(5, values);
}