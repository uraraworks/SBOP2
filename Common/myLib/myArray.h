#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <vector>

#include "myString.h"

//------------------------------------------------------------------------------
// MFC 風の CmyArray を std::vector ベースのラッパークラスに置き換え
//------------------------------------------------------------------------------
template <class TYPE, class ARG_TYPE = TYPE>
class CStdArray : public std::vector<TYPE>
{
public:
    using base_type = std::vector<TYPE>;
    using typename base_type::size_type;
    using base_type::begin;
    using base_type::cbegin;
    using base_type::cend;
    using base_type::data;
    using base_type::end;
    using base_type::operator[];

    CStdArray() = default;
    explicit CStdArray(size_type count) : base_type(count) {}
    CStdArray(size_type count, const TYPE &value) : base_type(count, value) {}
    CStdArray(std::initializer_list<TYPE> init) : base_type(init) {}

    int GetSize() const
    {
        return static_cast<int>(this->size());
    }

    void RemoveAll()
    {
        this->clear();
    }

    TYPE GetAt(int nIndex) const
    {
        if (nIndex < 0 || static_cast<size_type>(nIndex) >= this->size()) {
            return TYPE();
        }
        return (*this)[static_cast<size_type>(nIndex)];
    }

    void SetAt(int nIndex, ARG_TYPE newElement)
    {
        if (nIndex < 0 || static_cast<size_type>(nIndex) >= this->size()) {
            return;
        }
        (*this)[static_cast<size_type>(nIndex)] = newElement;
    }

    int Add(ARG_TYPE newElement)
    {
        this->push_back(newElement);
        return GetSize() - 1;
    }

    void InsertAt(int nIndex, ARG_TYPE newElement)
    {
        if (nIndex < 0) {
            nIndex = 0;
        }
        const size_type requested = static_cast<size_type>(nIndex);
        const size_type pos = std::min(requested, this->size());
        this->insert(this->begin() + static_cast<std::ptrdiff_t>(pos), newElement);
    }

    void RemoveAt(int nIndex, int nCount = 1)
    {
        if (nIndex < 0 || nCount <= 0) {
            return;
        }
        const size_type start = static_cast<size_type>(nIndex);
        if (start >= this->size()) {
            return;
        }

        size_type end = start + static_cast<size_type>(nCount);
        if (end > this->size()) {
            end = this->size();
        }
        this->erase(this->begin() + static_cast<std::ptrdiff_t>(start),
                    this->begin() + static_cast<std::ptrdiff_t>(end));
    }

    void Copy(const CStdArray *pSrc)
    {
        if (!pSrc) {
            RemoveAll();
            return;
        }

        this->operator=(*pSrc);
    }
};

using ARRAYINT = CStdArray<int, int>;
using PARRAYINT = ARRAYINT *;
using ARRAYDWORD = CStdArray<DWORD, DWORD>;
using PARRAYDWORD = ARRAYDWORD *;
using ARRAYBYTE = CStdArray<BYTE, BYTE>;
using PARRAYBYTE = ARRAYBYTE *;
using CmyStringArray = CStdArray<CmyString, const CmyString &>;
