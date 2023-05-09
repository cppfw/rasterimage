#pragma once

#include <r4/vector.hpp>

namespace rasterimage{

class image_base{
    r4::vector2<uint32_t> dimensions;
public:

    const decltype(dimensions)& dims()const noexcept{
        return this->dimensions;
    }
};

}
