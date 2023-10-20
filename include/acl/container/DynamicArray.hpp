#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <utility>
#include <stdio.h>
#include <malloc.h>

namespace acl
{
    /**
    * @class acl::DynamicArray<_Ty>
    * @brief random access, variable-size list data structure that allows elements to be added or removed.
    */
    template <typename _Ty>
    class DynamicArray
    {
        /**
        * @class acl::DynamicArray<_Ty>::DynArrIterator
        * @brief the basic iterator class for dynamic array of acl lib.
        */
        template<typename DynArray>
        class DynArrIterator
        {
        public:
            using valueType = typename DynArray::valueType;
            using iterator = typename DynArray::iterator;


        private:
            valueType* mpPtr = nullptr;
            valueType* mpBegin = nullptr;
            valueType* mpEnd = nullptr;

        public:
            /**
            * @brief the basic iterator default constructor.
            */
            DynArrIterator() noexcept {};

            /**
            * @brief the basic iterator param constructor.
            * @param[in] aPtr - pointer to the current element of the dynamic array handled by iterator.
            * @param[in] aBegin - pointer to the first element of the dynamic array.
            * @param[in] aEnd - pointer to the last element of the dynamic array.
            */
            DynArrIterator(valueType* aPtr, valueType* aBegin, valueType* aEnd)
                : mpPtr(aPtr), mpBegin(aBegin), mpEnd(aEnd)
            {
            }

            /**
            * @brief returns value from the currently handled pointer.
            * @returns value from the pointer.
            */
            inline const valueType& get() const
            {
                return *mpPtr;
            }

            /**
            * @brief sets value to the currently handled pointer.
            */
            inline void set(const valueType& atyValue)
            {
                *mpPtr = atyValue;
            }

            /**
            * @brief moves pointer to the next element of the dynamic array.
            */
            inline void next()
            {
                mpPtr++;
            }

            /**
            * @brief checks whether the next element is the last one in the array.
            * @returns **true** if next pointer can be moved forward and **false** if not.
            */
            bool hasNext() const
            {
                if (mpPtr == mpEnd) {
                    return false;
                }
                return true;
            }

            bool operator == (const DynArrIterator& it) const { return mpPtr == it.mpPtr; }
            bool operator != (const DynArrIterator& it) const { return !(*this == it); }
        };

        /**
        * @class acl::DynamicArray<_Ty>::DynArrReverseIterator
        * @brief the reverse iterator class for dynamic array of acl lib.
        */
        template<typename DynArray>
        class DynArrReverseIterator
        {
        public:
            using valueType = typename DynArray::valueType;
            using iterator = typename DynArray::riterator;


        private:
            valueType* mpPtr = nullptr;
            valueType* mpBegin = nullptr;
            valueType* mpEnd = nullptr;

        public:
            /**
            * @brief the reverse iterator default constructor.
            */
            DynArrReverseIterator() noexcept {};

            /**
            * @brief the reverse iterator param constructor.
            * @param[in] aPtr - pointer to the current element of the dynamic array handled by iterator.
            * @param[in] aBegin - pointer to the first element of the dynamic array.
            * @param[in] aEnd - pointer to the last element of the dynamic array.
            */
            DynArrReverseIterator(valueType* aPtr, valueType* aBegin, valueType* aEnd)
                : mpPtr(aPtr), mpBegin(aBegin), mpEnd(aEnd)
            {
            }

            /**
            * @brief returns value from the currently handled pointer.
            * @returns value from the pointer.
            */
            inline const valueType& get() const
            {
                return *mpPtr;
            }

            /**
            * @brief sets value to the currently handled pointer.
            */
            inline void set(const valueType& atyValue)
            {
                *mpPtr = atyValue;
            }

            /**
            * @brief moves pointer to the next element of the dynamic array from the end.
            */
            inline void next()
            {
                mpPtr--;
            }

            /**
            * @brief checks whether the next element is the first one in the array.
            * @returns **true** if next pointer can be moved backward and **false** if not.
            */
            bool hasNext() const
            {
                if (mpPtr == mpBegin) {
                    return false;
                }
                return true;
            }

            bool operator == (const DynArrReverseIterator& it) const { return mpPtr == it.mpPtr; }
            bool operator != (const DynArrReverseIterator& it) const { return !(*this == it); }
        };

    public:
        using valueType = _Ty;
        using indexType = size_t;
        using iterator = DynArrIterator<DynamicArray<_Ty>>;
        using riterator = DynArrReverseIterator<DynamicArray<_Ty>>;

    private:
        valueType* mArrayBuffer     = nullptr;
        indexType mnCurrentIndex    = NULL;
        indexType mSize             = NULL;
        indexType mCapacity         = NULL;

    private:
        /**
        * @brief exponentially reallocates memory in the array buffer.
        * @details exponentially reallocates memory in the array buffer,
        * multiplying its capacity by 2 each time it needs to be growth.
        */
        void resize() {
            mCapacity *= 2;

            valueType* newArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mSize; i++) {
                new (&newArrayBuffer[i]) valueType(std::move(mArrayBuffer[i]));
                mArrayBuffer[i].~valueType();
            }

            free(mArrayBuffer);
            mArrayBuffer = newArrayBuffer;
        }

    public:
        /**
        * @brief default dynamic array constructor. Allocate array buffer by the capacity value of 16
        */
        DynamicArray()
        {
            mSize = 0;
            mCapacity = 16;
            mArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mCapacity; ++i) {
                new (&mArrayBuffer[i]) valueType();
            }
        };

        /**
        * @brief copies all the element of given dynamic array to the new dynamic array object.
        * @param[in] aCopy - other object of the acl::DynamicArray class.
        */
        DynamicArray(const DynamicArray<valueType>& aCopy)
        {
            mSize = aCopy.mSize;
            mCapacity = aCopy.mSize * 2;
            mArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mSize; i++) {
                mArrayBuffer[i] = aCopy[i];
            }
        }

        /**
        * @brief allocates array buffer with the given size.
        * @param[in] aSize - initial size of the array buffer.
        */
        explicit DynamicArray(int aSize)
        {
            mSize = aSize;
            mCapacity = mSize * 2;
            mArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mCapacity; ++i) {
                new (&mArrayBuffer[i]) valueType();
            }
        };

        /**
        * @brief deallocates all the data from the memory of array buffer.
        */
        ~DynamicArray()
        {
            for (size_t i{}; i < mCapacity; ++i) {
                mArrayBuffer[i].~valueType();
            }
        }

        /**
        * @brief inserts element at the end of array buffer.
        * @param[in] aValue - value to place at the end of the array.
        * @returns index of insertion.
        */
        indexType insert(const valueType& aValue)
        {
            if (mSize == mCapacity) {
                resize();
            }

            mArrayBuffer[mSize] = aValue;
            mSize += 1;

            return mSize - 1;
        }

        /**
        * @brief inserts an element at the given array index.
        * Reorders an array buffer using a new value.
        * @param[in] aValue - value to place at the end of the array.
        * @returns index of insertion.
        */
        indexType insert(int aIndex, const valueType& aValue)
        {
            if (mSize == mCapacity) {
                resize();
            }

            if (aIndex == mSize) {
                return insert(aValue);
            }

            for (indexType i = mSize; i > aIndex; i--) {
                new (&mArrayBuffer[i]) valueType(std::move(mArrayBuffer[i - 1]));
                mArrayBuffer[i - 1].~valueType();
            }

            mSize += 1;
            mArrayBuffer[aIndex] = aValue;
            return aIndex;
        }

        /**
        * @brief removes element from the given index of the array buffer.
        * @param[in] aValue - index with value that needs to be removed.
        */
        void remove(int anIndex)
        {
            for (size_t i(anIndex + 1); i < size(); i++) {
                mArrayBuffer[i].~valueType();
                new (&mArrayBuffer[i - 1]) valueType(std::move(mArrayBuffer[i]));
            }

            mSize--;
        }

        /**
        * @brief returns dynamic array iterator points to the first element of the dynamic array.
        * @returns dynamic array iterator points to the first element of the dynamic array.
        */
        iterator begin()
        {
            return iterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns const dynamic array iterator points to the first element of the dynamic array.
        * @returns const dynamic array iterator points to the first element of the dynamic array.
        */
        const iterator begin() const
        {
            return iterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns dynamic array iterator points to the last element of the dynamic array.
        * @returns dynamic array iterator points to the last element of the dynamic array.
        */
        iterator end()
        {
            return iterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns const dynamic array iterator points to the last element of the dynamic array.
        * @returns const dynamic array iterator points to the last element of the dynamic array.
        */
        const iterator end() const
        {
            return iterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns dynamic array reverse iterator points to the first element of the dynamic array.
        * @returns dynamic array reverse iterator points to the first element of the dynamic array.
        */
        riterator rbegin()
        {
            return riterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns dynamic array const reverse iterator points to the first element of the dynamic array.
        * @returns dynamic array const reverse iterator points to the first element of the dynamic array.
        */
        const riterator rbegin() const
        {
            return riterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns dynamic array reverse iterator points to the last element of the dynamic array.
        * @returns dynamic array reverse iterator points to the last element of the dynamic array.
        */
        riterator rend()
        {
            return riterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns dynamic array const reverse iterator points to the last element of the dynamic array.
        * @returns dynamic array const reverse iterator points to the last element of the dynamic array.
        */
        const riterator rend() const
        {
            return riterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        /**
        * @brief returns the current size of the dynamic array.
        * @returns current size of the dynamic array.
        */
        indexType size() const { return mSize; }

        /**
        * @brief returns the current capacity of the dynamic array.
        * @returns current capacity of the dynamic array.
        */
        indexType capacity() const { return mCapacity; }

        /**
        * @brief checks if array has elements.
        * @returns **true** if at least one element exists in arrray buffer, **false** if not.
        */
        bool empty() const { return mSize == 0; }

        const valueType& operator [] (indexType anIndex) const { return mArrayBuffer[anIndex]; }
        valueType& operator [] (indexType anIndex) { return mArrayBuffer[anIndex]; }
    };
}
#endif // !DYNAMIC_ARRAY_HPP
