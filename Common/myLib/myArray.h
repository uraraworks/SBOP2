#pragma once

#include <vector>

#include "myString.h"

//------------------------------------------------------------------------------
// Legacy array aliases backed by std::vector
//------------------------------------------------------------------------------
template <class TYPE>
using CStdArray = std::vector<TYPE>;

using ARRAYINT = std::vector<int>;
using PARRAYINT = ARRAYINT *;
using ARRAYDWORD = std::vector<DWORD>;
using PARRAYDWORD = ARRAYDWORD *;
using ARRAYBYTE = std::vector<BYTE>;
using PARRAYBYTE = ARRAYBYTE *;
using CmyStringArray = std::vector<CmyString>;
