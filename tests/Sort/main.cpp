#include <acl/testing/PerformanceProfiler.hpp>
#include <acl/sorting/sort.hpp>
#include <gtest/gtest.h>
#include <random>

TEST(QuickSort, BasicSortingTest) {
    srand(0);

    int size = 1000;
    int* foo = new int[size];
    int* bar = new int[size];

    for (size_t i{}; i < size; i++) {
        int randint = rand() % 100;
        foo[i] = bar[i] = randint;
    }

    std::sort(bar, bar + size);
    acl::quickSort(foo, foo + size - 1, [&](int a, int b) { return acl::compLeftSmaller(a, b); });

    for (size_t i{}; i < size; i++) {
        EXPECT_EQ(bar[i], foo[i]);
    }
}

TEST(InsertSort, BasicSortingTest) {
    srand(0);

    int size = 1000;
    int* foo = new int[size];
    int* bar = new int[size];

    for (size_t i{}; i < size; i++) {
        int randint = rand() % 100;
        foo[i] = bar[i] = randint;
    }

    std::sort(bar, bar + size);
    acl::insertionSort(foo, foo + size - 1, [&](int a, int b) { return acl::compLeftSmaller(a, b); });

    for (size_t i{}; i < size; i++) {
        EXPECT_EQ(bar[i], foo[i]);
    }
}

TEST(GeneralizedSort, BasicSortingTest) {
    srand(0);

    int size = 1000;
    int* foo = new int[size];
    int* bar = new int[size];

    for (size_t i{}; i < size; i++) {
        int randint = rand() % 100;
        foo[i] = bar[i] = randint;
    }

    std::sort(bar, bar + size);
    acl::sort(foo, foo + size - 1, [&](int a, int b) { return acl::compLeftSmaller(a, b); });

    for (size_t i{}; i < size; i++) {
        EXPECT_EQ(bar[i], foo[i]);
    }
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}