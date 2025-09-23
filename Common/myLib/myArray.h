#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "myString.h"

//------------------------------------------------------------------------------
// STL ベースの配列ヘルパークラス
//------------------------------------------------------------------------------
template <class TYPE>
class CStdArray : public std::vector<TYPE>
{
public:
    using base_type = std::vector<TYPE>;
    using size_type = typename base_type::size_type;
    using difference_type = typename base_type::difference_type;

    using base_type::base_type;

    int GetSize() const
    {
        return static_cast<int>(base_type::size());
    }

    void RemoveAll()
    {
        base_type::clear();
        base_type::shrink_to_fit();
    }

    TYPE GetAt(int nIndex) const
    {
        if (nIndex < 0 || static_cast<size_type>(nIndex) >= base_type::size())
        {
            return TYPE{};
        }
        return base_type::operator[](static_cast<size_type>(nIndex));
    }

    void SetAt(int nIndex, TYPE newElement)
    {
        if (nIndex < 0 || static_cast<size_type>(nIndex) >= base_type::size())
        {
            return;
        }
        base_type::operator[](static_cast<size_type>(nIndex)) = std::move(newElement);
    }

    int Add(TYPE newElement)
    {
        base_type::push_back(std::move(newElement));
        return static_cast<int>(base_type::size() - 1);
    }

    void InsertAt(int nIndex, TYPE newElement)
    {
        if (nIndex < 0)
        {
            nIndex = 0;
        }
        const size_type idx = std::min<size_type>(static_cast<size_type>(nIndex), base_type::size());
        base_type::insert(base_type::begin() + static_cast<difference_type>(idx), std::move(newElement));
    }

    void RemoveAt(int nIndex, int nCount = 1)
    {
        if (nCount <= 0 || nIndex < 0 || base_type::empty())
        {
            return;
        }

        const size_type start = static_cast<size_type>(nIndex);
        if (start >= base_type::size())
        {
            return;
        }

        const size_type count = std::min<size_type>(static_cast<size_type>(nCount), base_type::size() - start);
        base_type::erase(
            base_type::begin() + static_cast<difference_type>(start),
            base_type::begin() + static_cast<difference_type>(start + count));
    }

    void Copy(const CStdArray &src)
    {
        base_type::operator=(src);
    }

    void Copy(const CStdArray *pSrc)
    {
        if (pSrc == nullptr)
        {
            RemoveAll();
            return;
        }
        base_type::operator=(*pSrc);
    }
};

using ARRAYINT = CStdArray<int>;
using PARRAYINT = ARRAYINT *;
using ARRAYDWORD = CStdArray<DWORD>;
using PARRAYDWORD = ARRAYDWORD *;
using ARRAYBYTE = CStdArray<BYTE>;
using PARRAYBYTE = ARRAYBYTE *;
using CmyStringArray = CStdArray<CmyString>;
