#pragma once

#include "image_base.hpp"

namespace rasterimage{

template <typename channel_type, size_t num_channels>
class static_format_image : public image_base{
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
public:
};

}
