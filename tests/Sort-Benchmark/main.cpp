#include <iostream>
#include <fstream>
#include <windows.h>
#include <profileapi.h>

#include <acl/sorting/sort.hpp>

void benchmark() {
    LARGE_INTEGER frequency{ 0 };
    LARGE_INTEGER initTicks{ 0 };
    LARGE_INTEGER currTicks{ 0 };

    std::ofstream outputFileQuickSort;
    std::ofstream outputFileInsertSort;
    std::ofstream outputFileGeneralSort;

    outputFileQuickSort.open("benchmark-quickSorting.txt", std::ios_base::trunc);
    outputFileInsertSort.open("benchmark-insertSorting.txt", std::ios_base::trunc);
    outputFileGeneralSort.open("benchmark-generalSorting.txt", std::ios_base::trunc);

    if (!outputFileQuickSort.is_open() || !outputFileInsertSort.is_open() || !outputFileGeneralSort.is_open())
    {
        std::cout << "File does not exists" << std::endl;
    }
    else
    {
        QueryPerformanceFrequency(&frequency);
        for (int size = 2; size <= 50; size++)
        {
            int* arr = new int[size];
            for (int i{}; i < size; i++)
            {
                arr[i] = i;

            }
            QueryPerformanceCounter(&initTicks);
            for (int i{}; i < 10000; i++)
            {
                acl::quickSort(arr, arr + size - 1, [](int a, int b) { return acl::compLeftLarger(a, b); });
                acl::quickSort(arr, arr + size - 1, [](int a, int b) { return acl::compLeftSmaller(a, b); });
            }
            QueryPerformanceCounter(&currTicks);
            outputFileQuickSort << size << "\t"
                << (double)((currTicks.QuadPart - initTicks.QuadPart) * 1000000 / frequency.QuadPart) << std::endl;


            QueryPerformanceCounter(&initTicks);
            for (int i{}; i < 10000; i++)
            {
                acl::insertionSort(arr, arr + size - 1, [](int a, int b) { return acl::compLeftLarger(a, b); });
                acl::insertionSort(arr, arr + size - 1, [](int a, int b) { return acl::compLeftSmaller(a, b); });
            }
            QueryPerformanceCounter(&currTicks);
            outputFileInsertSort << size << "\t"
                << (double)((currTicks.QuadPart - initTicks.QuadPart) * 1000000 / frequency.QuadPart) << std::endl;


            QueryPerformanceCounter(&initTicks);
            for (int i{}; i < 10000; i++)
            {
                acl::quickSort(arr, arr + size - 1, [](int a, int b) { return acl::compLeftLarger(a, b); });
                acl::quickSort(arr, arr + size - 1, [](int a, int b) { return acl::compLeftSmaller(a, b); });
            }
            QueryPerformanceCounter(&currTicks);
            outputFileGeneralSort << size << "\t"
                << (double)((currTicks.QuadPart - initTicks.QuadPart) * 1000000 / frequency.QuadPart) << std::endl;

            delete[] arr;
        }
        outputFileQuickSort.close();
        outputFileInsertSort.close();
        outputFileGeneralSort.close();
    }

}

int main()
{
    benchmark();
}