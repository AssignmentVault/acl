#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <utility>
#include <stdio.h>
#include <malloc.h>

namespace acl
{
    template <typename _Ty>
    class DynamicArray
    {
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
            DynArrIterator() noexcept {};
            DynArrIterator(valueType* apPtr, valueType* apBegin, valueType* apEnd)
                : mpPtr(apPtr), mpBegin(apBegin), mpEnd(apEnd)
            {
            }

            inline const valueType& get() const
            {
                return *mpPtr;
            }

            inline void set(const valueType& atyValue)
            {
                *mpPtr = atyValue;
            }

            inline void next()
            {
                mpPtr++;
            }

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

    public:
        using valueType = _Ty;
        using indexType = size_t;
        using iterator = DynArrIterator<DynamicArray<_Ty>>;

    private:
        valueType* mArrayBuffer     = nullptr;
        indexType mnCurrentIndex    = NULL;
        indexType mSize             = NULL;
        indexType mCapacity         = NULL;

    private:
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
        DynamicArray()
        {
            mSize = 0;
            mCapacity = 16;
            mArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mCapacity; ++i) {
                new (&mArrayBuffer[i]) valueType();
            }
        };

        DynamicArray(const DynamicArray<valueType>& aCopy)
        {
            mSize = aCopy.mSize;
            mCapacity = aCopy.mSize * 2;
            mArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mSize; i++) {
                mArrayBuffer[i] = aCopy[i];
            }
        }

        explicit DynamicArray(int aSize)
        {
            mSize = aSize;
            mCapacity = mSize * 2;
            mArrayBuffer = static_cast<valueType*>(malloc(sizeof(valueType) * mCapacity));
            for (indexType i{}; i < mCapacity; ++i) {
                new (&mArrayBuffer[i]) valueType();
            }
        };

        ~DynamicArray()
        {
            for (size_t i{}; i < mCapacity; ++i) {
                mArrayBuffer[i].~valueType();
            }
        }

        indexType insert(const valueType& aValue)
        {
            if (mSize == mCapacity) {
                resize();
            }

            mArrayBuffer[mSize] = aValue;
            mSize += 1;

            return mSize - 1;
        }

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

        void remove(int anIndex)
        {
            for (size_t i(anIndex + 1); i < size(); i++) {
                mArrayBuffer[i].~valueType();
                new (&mArrayBuffer[i - 1]) valueType(std::move(mArrayBuffer[i]));
            }

            mSize--;
        }

        iterator begin()
        {
            return iterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        const iterator begin() const
        {
            return iterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        iterator end()
        {
            return iterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        const iterator end() const
        {
            return iterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        indexType size() const { return mSize; }
        indexType capacity() const { return mCapacity; }
        bool empty() const { return mSize == 0; }

        const valueType& operator [] (indexType anIndex) const { return mArrayBuffer[anIndex]; }
        valueType& operator [] (indexType anIndex) { return mArrayBuffer[anIndex]; }
    };
}
#endif // !DYNAMIC_ARRAY_HPP
