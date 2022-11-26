#pragma once

#include <iostream>
#include <string>
#include <cmath>

namespace se
{

    class Maths
    {
    public:
        template <typename T>
        static T lerp(const T &a, const T &b, float t)
        {
            return a + (b - a) * t;
        }
    };

} // namespace se
