#include <acl/container/DynamicArray.hpp>
#include <gtest/gtest.h>
#include <typeinfo>

TEST(DynamicArray, ConstructDynamicArrayByDefault) {
    acl::DynamicArray<int> foo;

    EXPECT_EQ(0, foo.size());
    EXPECT_EQ(0, foo.capacity());
}

TEST(DynamicArray, ConstructDynamicArrayWithSize) {
    acl::DynamicArray<int> foo(25);

    EXPECT_EQ(25, foo.size());
    EXPECT_EQ(50, foo.capacity());
}

TEST(DynamicArray, ConstructDynamicArrayString) {
    acl::DynamicArray<std::string> foo(10);

    EXPECT_EQ(typeid(foo[0]), typeid(std::string));
}

// Push

TEST(DynamicArray, DataInsertion) {
    acl::DynamicArray<int> foo(10);
    foo.insert(0, 10);
    foo.insert(1, 11);
    foo.insert(2, 12);

    foo.insert(25);
    foo.insert(35);

    EXPECT_EQ(foo[0], 10);
    EXPECT_EQ(foo[1], 11);
    EXPECT_EQ(foo[2], 12);

    EXPECT_EQ(foo[foo.size() - 2], 25);
    EXPECT_EQ(foo[foo.size() - 1], 35);
}

TEST(DynamicArray, EmptyInsert) {
    acl::DynamicArray<int> foo;
    foo.insert(25);

    EXPECT_EQ(foo[0], 25);
}

// Resize

TEST(DynamicArray, ResizingArray) {
    acl::DynamicArray<int> foo(2);
    int oldCapacity = foo.capacity();
    int oldSize = foo.size();

    foo.insert(1);
    foo.insert(2);
    foo.insert(3);

    int newCapacity = foo.capacity();
    int newSize = foo.size();

    EXPECT_NE(oldCapacity, newCapacity);
    EXPECT_NE(oldSize, newSize);
    EXPECT_EQ(foo.capacity(), 8);
}

// Iterator

TEST(DynamicArray, IteratorTest) {
    acl::DynamicArray<int> foo;

    foo.insert(1);
    foo.insert(2);
    foo.insert(3);
    foo.insert(4);
    foo.insert(5);
    EXPECT_EQ(1, foo.begin().get());

    foo[0] = 99;
    auto it = foo.begin();
    EXPECT_EQ(it.get(), 99);

    EXPECT_TRUE(it.hasNext());

    it.next();
    EXPECT_EQ(it.get(), 2);

    it.next();
    EXPECT_EQ(it.get(), 3);

    it.next();
    it.next();
    it.next();
    EXPECT_FALSE(it.hasNext());

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}