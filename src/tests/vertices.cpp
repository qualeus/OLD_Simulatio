#include "../../include/Geometry/Geometry.hpp"
#include "../../include/Geometry/String.hpp"

template <typename T>
void FunctionCentroid() {
    gmt::Vertices<T> ver = gmt::Vertices<T>({std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(4, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(4, 4)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 4))});

    gmt::Vector<T> test_result = ver.Centroid();
    gmt::Vector<T> true_result = gmt::Vector<T>(2, 2);
    ASSERT_EQUAL(true_result, test_result);
}

template <typename T>
void FunctionAverage() {
    gmt::Vertices<T> ver = gmt::Vertices<T>({std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(4, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(4, 4)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 4))});

    gmt::Vector<T> test_result = ver.Average();
    gmt::Vector<T> true_result = gmt::Vector<T>(2, 2);
    ASSERT_EQUAL(true_result, test_result);
}

template <typename T>
void FunctionConvex() {
    gmt::Vertices<T> ver = gmt::Vertices<T>({std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(4, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(4, 4)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 4))});

    bool test_result = ver.Convex();
    bool true_result = true;
    ASSERT_EQUAL(true_result, test_result);
}

template <typename T>
void FunctionArea() {
    gmt::Vertices<T> ver = gmt::Vertices<T>({std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(3, 5)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(0, 0)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(5, 2)), std::make_shared<gmt::Vector<T>>(gmt::Vector<T>(1, 1))});

    bool test_result = ver.Convex();
    bool true_result = false;
    ASSERT_EQUAL(true_result, test_result);
}

template <typename T>
void FunctionSizes() {}

template <typename T>
void FunctionBounds() {}

template <typename T>
void FunctionReorder() {}

template <typename T>
void FunctionTranslate() {}

template <typename T>
void FunctionRotate() {}

template <typename T>
void FunctionScale() {}

int main(int, char**) {
    FunctionCentroid<int>();
    FunctionCentroid<float>();
    FunctionCentroid<double>();

    FunctionAverage<int>();
    FunctionAverage<float>();
    FunctionAverage<double>();

    FunctionConvex<int>();
    FunctionConvex<float>();
    FunctionConvex<double>();

    FunctionArea<int>();
    FunctionArea<float>();
    FunctionArea<double>();

    FunctionSizes<int>();
    FunctionSizes<float>();
    FunctionSizes<double>();

    FunctionBounds<int>();
    FunctionBounds<float>();
    FunctionBounds<double>();

    FunctionReorder<int>();
    FunctionReorder<float>();
    FunctionReorder<double>();

    FunctionTranslate<int>();
    FunctionTranslate<float>();
    FunctionTranslate<double>();

    FunctionRotate<int>();
    FunctionRotate<float>();
    FunctionRotate<double>();

    FunctionScale<int>();
    FunctionScale<float>();
    FunctionScale<double>();
}
