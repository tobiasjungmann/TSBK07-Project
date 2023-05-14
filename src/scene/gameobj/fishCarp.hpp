#pragma once
#include "fish.hpp"

namespace obj
{
    class Carp : public Fish
    {
    public:
        using Fish::Fish;

        inline mat3 orientationMtx() const noexcept override
        {
            return {
                1, 0, 0,
                0, 1, 0,
                0, 0, 1};
        }
    };
}