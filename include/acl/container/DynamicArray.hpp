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
        valueType* mtyArrayBuffer = nullptr;
        indexType mnCurrentIndex = NULL;
        indexType mnBufferSize = NULL;
        indexType mnActualSize = NULL;

    public:
        DynamicArray()
        {
            mnBufferSize = 0;
            mnActualSize = 16;

            mtyArrayBuffer = static_cast<valueType*>(calloc(mnActualSize, sizeof(valueType) * mnActualSize));
        };

        DynamicArray(int anSize)
        {
            mnBufferSize = anSize;
            mnActualSize = mnBufferSize * 2;

            mtyArrayBuffer = static_cast<valueType*>(calloc(mnActualSize, sizeof(valueType) * mnActualSize));
        };

        ~DynamicArray()
        {
            for (size_t i{}; i < mnActualSize; i++) {
                mtyArrayBuffer[i].~valueType();
            }
        }

        int insert(const valueType& atyValue)
        {
            if (mnBufferSize + 1 >= mnActualSize) {
                mnActualSize += 10;
                mtyArrayBuffer = static_cast<valueType*>(realloc(mtyArrayBuffer, sizeof(valueType) * mnActualSize));
            }

            mtyArrayBuffer[mnBufferSize] = atyValue;
            mnBufferSize++;

            return mnBufferSize - 1;
        }

        int insert(int anIndex, const valueType& atyValue)
        {
            if (anIndex == mnBufferSize) {
                return insert(atyValue);
            }

            valueType* newBuffer = static_cast<valueType*>(calloc(mnActualSize, sizeof(valueType) * mnActualSize));
            for (indexType i{}; i < (anIndex + 1); i++) {
                newBuffer[i] = mtyArrayBuffer[i];
            }
            newBuffer[anIndex] = atyValue;

            for (indexType i(anIndex + 1); i < mnBufferSize + 1; i++) {
                newBuffer[i] = mtyArrayBuffer[i - 1];
            }

            free(mtyArrayBuffer);
            mtyArrayBuffer = std::move(newBuffer);
            mnBufferSize++;

            return anIndex;
        }

        void remove(int anIndex)
        {
            for (size_t i(anIndex + 1); i < size(); i++) {
                mtyArrayBuffer[i - 1] = std::move(mtyArrayBuffer[i]);
                mtyArrayBuffer[i] = 0;
            }

            mnBufferSize--;
        }

        inline iterator begin()
        {
            return iterator(mtyArrayBuffer, mtyArrayBuffer, mtyArrayBuffer + mnBufferSize);
        }

        inline const iterator begin() const
        {
            return iterator(mtyArrayBuffer, mtyArrayBuffer, mtyArrayBuffer + mnBufferSize);
        }

        inline iterator end()
        {
            return iterator(mtyArrayBuffer + mnBufferSize, mtyArrayBuffer, mtyArrayBuffer + mnBufferSize);
        }

        inline const iterator end() const
        {
            return iterator(mtyArrayBuffer + mnBufferSize, mtyArrayBuffer, mtyArrayBuffer + mnBufferSize);
        }

        inline indexType size() const { return mnBufferSize; }
        inline bool empty() const { return mnBufferSize == 0; }

        inline const valueType& operator [] (indexType anIndex) const { return mtyArrayBuffer[anIndex]; }
        inline valueType& operator [] (indexType anIndex) { return mtyArrayBuffer[anIndex]; }
    };
}
#endif // !DYNAMIC_ARRAY_HPP
