#pragma once
#include <math.h>

constexpr float PI = 3.14159265f;

template <typename T>
constexpr auto sq(const T& x)
{
    return x * x;
}

template<typename T>
constexpr T to_rad(T deg)
{
    return deg * PI / (T)180.0;
}