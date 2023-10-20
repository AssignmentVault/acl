#ifndef ACL_SORT_HPP
#define ACL_SORT_HPP

#include <utility>
#include <functional>

namespace acl
{

    /////////////////////////////
    /// Comparenment functions
    ////////////////////////////

    /**
    * @brief compare predicate for sorting functions.
    * @param[in] aLeft - pointer to the left element of compare.
    * @param[in] aRight - pointer to the right element of compare.
    * @returns true if **aLeft** less than **aRight**. False if not.
    */
    template<typename _Ty>
    bool compLeftSmaller(const _Ty& aLeft, const _Ty& aRight) {
        return (aLeft < aRight);
    }

    /**
    * @brief compare predicate for sorting functions.
    * @param[in] aLeft - pointer to the left element of compare.
    * @param[in] aRight - pointer to the right element of compare.
    * @returns true if **aLeft** larger than **aRight**. False if not.
    */
    template<typename _Ty>
    bool compLeftLarger(const _Ty& aLeft, const _Ty& aRight) {
        return (aLeft > aRight);
    }

    /////////////////////////////
    /// Generalized sorting
    ////////////////////////////

    /**
    * @brief **NO RETURN** implementation of insertion based generalized sorting algorithm.
    * @param[in] aBegin - pointer to the start of sorting area.
    * @param[in] aEnd - pointer to the end of sorting area.
    * @param[in] aComp - condition for comparing elements.
    * @note since function accepts pointers to the array elements, we don't return sorted array.
    * Function just sort the original one.
    */
    template<typename _Ty, typename _Cmp>
    void sort(_Ty* aBegin, _Ty* aEnd, _Cmp aComp) {
        if (aBegin >= aEnd) { return; }

        if (aEnd - aBegin > 10) {

            _Ty* first = aBegin;
            _Ty* last = aEnd;
            _Ty* middle = aBegin + (aEnd - aBegin) / 2;

            if (aComp(*aBegin, *aEnd)) {
                if (aComp(*aBegin, *middle)) {
                    if (aComp(*aEnd, *middle)) {
                        middle = aEnd;
                    }
                }
            }
            else {
                if (aComp(*aBegin, *middle)) {
                    middle = aBegin;
                }
                else {
                    if (aComp(*middle, *aEnd)) {
                        middle = aEnd;
                    }
                }
            }

            const _Ty currentValue = *middle;
            do {
                while (aComp(*first, currentValue)) {
                    first += 1;
                }
                while (aComp(currentValue, *last)) {
                    last -= 1;
                }

                if (first <= last) {
                    std::swap(*first, *last);
                    first++;
                    last--;
                }
            } while (first <= last);

            while (aBegin < aEnd)
            {
                if (last - aBegin > aEnd - first) {
                    quickSort(aBegin, last, aComp);
                    aBegin = first;
                }
                else
                {
                    quickSort(first, aEnd, aComp);
                    aEnd = last;
                }
            }
        }
        else {
            insertionSort(aBegin, aEnd, aComp);
        }
    }

    /////////////////////////////
    /// Sorting algorithms
    ////////////////////////////

    /**
    * @brief **NO RETURN** implementation of insertion sorting algorithm.
    * @details Insertion sort is a simple sorting algorithm
    * that works similar to the way you sort playing cards in your hands.
    * The array is virtually split into a sorted and an unsorted part.
    * Values from the unsorted part are picked and placed at the correct position in the sorted part.
    * @param[in] aBegin - pointer to the start of sorting area.
    * @param[in] aEnd - pointer to the end of sorting area.
    * @param[in] aComp - condition for comparing elements.
    * @see https://www.geeksforgeeks.org/insertion-sort/
    * @note since function accepts pointers to the array elements, we don't return sorted array.
    * Function just sort the original one.
    */
    template<typename _Ty, typename _Cmp>
    void insertionSort(_Ty* aBegin, _Ty* aEnd, _Cmp aComp) {
        if (aBegin == aEnd) { return; }
        for (_Ty* i(aBegin + 1); i <= aEnd; i++) {
            _Ty currentValue = *i;
            _Ty* prevElement = i - 1;
            while (prevElement >= aBegin && aComp(currentValue, *prevElement)) {
                *(prevElement + 1) = std::move(*prevElement);
                prevElement--;
            }
            *(prevElement + 1) = currentValue;
        }
    }

    /**
    * @brief **NO RETURN** implementation of quick sorting algorithm.
    * @details QuickSort is a sorting algorithm based on the Divide and Conquer algorithm that
    * picks an element as a pivot and partitions the given array around the picked pivot by placing
    * the pivot in its correct position in the sorted array.
    * @param[in] aBegin - pointer to the start of sorting area.
    * @param[in] aEnd - pointer to the end of sorting area.
    * @param[in] aComp - condition for comparing elements.
    * @see https://www.geeksforgeeks.org/quick-sort/
    * @note since function accepts pointers to the array elements, we don't return sorted array.
    * Function just sort the original one.
    */
    template<typename _Ty, typename _Cmp>
    void quickSort(_Ty* aBegin, _Ty* aEnd, _Cmp aComp) {
        if (aBegin == aEnd) { return; }

        _Ty* first  = aBegin;
        _Ty* last   = aEnd;
        _Ty* middle = aBegin + (aEnd - aBegin) / 2;

        if (aComp(*aBegin, *aEnd)) {
            if (aComp(*aBegin, *middle)) {
                if (aComp(*aEnd, *middle)) {
                    middle = aEnd;
                }
            }
        }
        else {
            if (aComp(*aBegin, *middle)) {
                middle = aBegin;
            }
            else {
                if (aComp(*middle, *aEnd)) {
                    middle = aEnd;
                }
            }
        }

        const _Ty currentValue = *middle;
        do {
            while (aComp(*first, currentValue)) {
                first += 1;
            }
            while (aComp(currentValue, *last)) {
                last -= 1;
            }

            if (first <= last) {
                std::swap(*first, *last);
                first++;
                last--;
            }
        } while (first <= last);

        while (aBegin < aEnd)
        {
            if (last - aBegin > aEnd - first) {
                quickSort(aBegin, last, aComp);
                aBegin = first;
            }
            else
            {
                quickSort(first, aEnd, aComp);
                aEnd = last;
            }
        }
    }
}

#endif // !ACL_SORT_HPP