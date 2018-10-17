#include <gtest/gtest.h>
#include "vector_basic_test.hpp"
// #include "VectorTests.cpp"

TEST(TestTest, TestingTrue)
{
    ASSERT_TRUE(true);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}