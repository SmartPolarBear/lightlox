//
// Created by bear on 2/4/2023.
//

#pragma once
#include <experimental/generator>

namespace lightlox
{
template<class T, class TAlloc = std::allocator<char>>
using generator = std::experimental::generator<T, TAlloc>;
}