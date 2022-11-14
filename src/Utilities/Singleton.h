#pragma once

#include "Logger.h"

namespace se
{
    template <class T>
    class Singletonizer
    {
    public:
        /* data */
        Singletonizer() = default;
        ~Singletonizer() = default;

        Singletonizer(const Singletonizer &) = delete;
        Singletonizer &operator=(const Singletonizer &) = delete;
        Singletonizer(Singletonizer &&) = delete;
        Singletonizer &operator=(Singletonizer &&) = delete;

        static T &Get()
        {
            static T instance;
            return instance;
        }
    };

}