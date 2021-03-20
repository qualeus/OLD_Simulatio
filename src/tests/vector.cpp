#include "../../include/Geometry/Geometry.hpp"
#include "../../include/tests/test.hpp"

void IntOperatorPlus() {
    gmt::Vector<float> lhs = gmt::Vector<float>(1, 2);
    gmt::Vector<float> rhs = gmt::Vector<float>(1, 3);

    gmt::Vector<float> res = gmt::Vector<float>(2, 5);

    ASSERT_THROW((lhs + rhs) == res);
    ASSERT_THROW((rhs + lhs) == res);
    ASSERT_THROW((rhs + rhs) != res);
}

void IntOperatorMinus() {
    gmt::Vector<float> lhs = gmt::Vector<float>(1, 2);
    gmt::Vector<float> rhs = gmt::Vector<float>(1, 3);

    gmt::Vector<float> resA = gmt::Vector<float>(0, -1);
    gmt::Vector<float> resB = gmt::Vector<float>(0, 1);
    gmt::Vector<float> resC = gmt::Vector<float>(0, 0);

    ASSERT_THROW((lhs - rhs) == resA);
    ASSERT_THROW((rhs - lhs) == resB);
    ASSERT_THROW((rhs - rhs) == resC);
}

int main(int, char**) {
    IntOperatorPlus();
    IntOperatorMinus();
}