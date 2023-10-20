#include <acl/sorting/sort.hpp>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    //testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    int* mas = new int[5];
    mas[0] = 9;
    mas[1] = 2;
    mas[2] = 0;
    mas[3] = 1;
    mas[4] = 6;

    //acl::insertionSort(mas, mas + 4, [&](int a, int b) { return (a < b); });
    acl::sort(mas, mas + 4, [&](int a, int b) { return acl::compLeftSmaller(a, b); });
    for (size_t i{}; i < 5; i++) {
        std::cout << mas[i] << std::endl;
    }
}