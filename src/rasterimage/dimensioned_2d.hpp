#pragma once

#include <r4/vector.hpp>

namespace rasterimage{

template <typename dimension_type>
class dimensioned_2d{
protected:
    r4::vector2<dimension_type> dimensions;
public:
    dimensioned_2d() = default;

    dimensioned_2d(const decltype(dimensions)& d) :
        dimensions(d)
    {}

    const decltype(dimensions)& dims()const noexcept{
        return this->dimensions;
    }
};

}
