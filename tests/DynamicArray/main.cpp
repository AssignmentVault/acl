#include <acl/container/DynamicArray.hpp>
#include <gtest/gtest.h>
#include <typeinfo>

TEST(DynamicArray, ConstructDynamicArrayByDefault) {
    acl::DynamicArray<int> foo;

    EXPECT_EQ(0, foo.size());
    EXPECT_EQ(16, foo.capacity());
}

TEST(DynamicArray, ConstructDynamicArrayWithSize) {
    acl::DynamicArray<int> foo(25);

    EXPECT_EQ(25, foo.size());
    EXPECT_EQ(50, foo.capacity());
}

TEST(DynamicArray, ConstructWithMoveConstructor) {
    acl::DynamicArray<int> foo;
    for (size_t i{}; i < 10; i++) {
        foo.insert(i + 1);
    }

    acl::DynamicArray<int> bar(std::move(foo));
    for (size_t i{}; i < 10; i++) {
        EXPECT_EQ(bar[i], i + 1);
    }
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

TEST(DynamicArray, RemoveDataTest) {
    acl::DynamicArray<int> foo;
    foo.insert(1);
    foo.insert(2);
    foo.insert(3);
    foo.insert(4);
    foo.insert(5);

    foo.remove(3);
    EXPECT_EQ(foo[3], 5);
}

// Strings

TEST(DynamicArray, ConstructDynamicArrayString) {
    acl::DynamicArray<std::string> foo(10);

    EXPECT_EQ(typeid(foo[0]), typeid(std::string));
}

TEST(DynamicArray, CopyConstructorStringTest) {
    acl::DynamicArray<std::string> foo(5);
    foo.insert(std::string("1"));
    foo.insert(std::string("2"));
    foo.insert(std::string("3"));
    foo.insert(std::string("4"));
    foo.insert(std::string("5"));

    acl::DynamicArray<std::string> bar(foo);
    for (size_t i{}; i < bar.size(); i++) {
        EXPECT_EQ(bar[i], foo[i]);
    }
}

TEST(DynamicArray, RemoveStringDataTest) {
    acl::DynamicArray<std::string> foo;
    foo.insert(std::string("1"));
    foo.insert(std::string("2"));
    foo.insert(std::string("3"));
    foo.insert(std::string("4"));
    foo.insert(std::string("5"));

    foo.remove(3);
    EXPECT_EQ(foo[3], "5");
}

TEST(DynamicArray, InsertStringDataTest) {
    acl::DynamicArray<std::string> foo;
    foo.insert(std::string("1"));
    foo.insert(std::string("2"));
    foo.insert(std::string("3"));
    foo.insert(std::string("4"));
    foo.insert(std::string("5"));

    foo.insert(2, "148");
    EXPECT_EQ(foo[0], "1");
    EXPECT_EQ(foo[1], "2");
    EXPECT_EQ(foo[2], "148");
    EXPECT_EQ(foo[3], "3");
    EXPECT_EQ(foo[4], "4");
    EXPECT_EQ(foo[5], "5");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}