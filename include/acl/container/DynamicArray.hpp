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
            std::copy(mArrayBuffer, mArrayBuffer + mSize, newArrayBuffer);
            for (indexType i{}; i < mSize; i++) {
                newArrayBuffer[i] = std::move(mArrayBuffer[i]);
            }

            for (indexType i{}; i < mSize; ++i) {
                mArrayBuffer[i].~valueType();
            }

            free(mArrayBuffer);
            mArrayBuffer = newArrayBuffer;
        }

    public:
        DynamicArray()
        {
            mSize = 0;
            mCapacity = 0;
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

        DynamicArray(int aSize)
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
                mArrayBuffer[i] = mArrayBuffer[i - 1];
            }

            mSize += 1;
            mArrayBuffer[aIndex] = aValue;
            return aIndex;
        }

        void remove(int anIndex)
        {
            for (size_t i(anIndex + 1); i < size(); i++) {
                mArrayBuffer[i - 1] = std::move(mArrayBuffer[i]);
                mArrayBuffer[i] = 0;
            }

            mSize--;
        }

        inline iterator begin()
        {
            return iterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        inline const iterator begin() const
        {
            return iterator(mArrayBuffer, mArrayBuffer, mArrayBuffer + mSize);
        }

        inline iterator end()
        {
            return iterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        inline const iterator end() const
        {
            return iterator(mArrayBuffer + mSize, mArrayBuffer, mArrayBuffer + mSize);
        }

        inline indexType size() const { return mSize; }
        inline indexType capacity() const { return mCapacity; }
        inline bool empty() const { return mSize == 0; }

        inline const valueType& operator [] (indexType anIndex) const { return mArrayBuffer[anIndex]; }
        inline valueType& operator [] (indexType anIndex) { return mArrayBuffer[anIndex]; }
    };
}
#endif // !DYNAMIC_ARRAY_HPP
