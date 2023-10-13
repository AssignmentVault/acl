#include <acl/container/DynamicArray.hpp>
#include <gtest/gtest.h>

TEST(DynamicArrayTest, ArrayEmptyness) {
    acl::DynamicArray<int> foo;
    EXPECT_TRUE(foo.empty());
}

TEST(DynamicArrayTest, ArrayInsertion) {
    acl::DynamicArray<int> foo(5);

    foo.insert(25);
    EXPECT_EQ(25, foo[5]);

    foo.insert(3, 66);
    EXPECT_EQ(66, foo[3]);
}

TEST(DynamicArrayTest, InitArrayWithGivenSize) {
    acl::DynamicArray<int> foo(100);
    EXPECT_EQ(100, foo.size());
}

TEST(DynamicArrayTest, InitialDynamicArrayState) {
    acl::DynamicArray<int> foo;

    for (size_t i{}; i < foo.size(); i++) {
        EXPECT_EQ(0, foo[i]);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}