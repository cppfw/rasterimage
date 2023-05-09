#pragma once

#include <r4/vector.hpp>

namespace rasterimage{

template <typename channel_type, size_t num_channels>
class static_format_image{
public:
    using pixel_type =
        std::enable_if_t<1 <= num_channels || num_channels <= 4, 
            std::conditional_t<num_channels == 1,
                channel_type,
                r4::vector<channel_type, num_channels>   
            >
        >;

private:
    std::vector<pixel_type> buffer;

    r4::vector2<uint32_t> dimensions;
public:

    const decltype(dimensions)& dims()const noexcept{
        return this->dimensions;
    }
};

}
