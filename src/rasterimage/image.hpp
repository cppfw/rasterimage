#pragma once

#include <array>
#include <variant>

#include <r4/vector.hpp>

namespace rasterimage{

template <typename channel_type>
class channel{
    static_assert(
        std::is_same_v<channel_type, uint8_t> ||
        std::is_same_v<channel_type, uint16_t> ||
        std::is_same_v<channel_type, float>
        , "only uint8_t, uint16_t or float are supported");
public:
    channel_type value;

};

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

class image{
    std::variant<
        static_format_image<uint8_t, 1>,
        static_format_image<uint8_t, 2>,
        static_format_image<uint8_t, 3>,
        static_format_image<uint8_t, 4>,
        static_format_image<uint16_t, 1>,
        static_format_image<uint16_t, 2>,
        static_format_image<uint16_t, 3>,
        static_format_image<uint16_t, 4>,
        static_format_image<float, 1>,
        static_format_image<float, 2>,
        static_format_image<float, 3>,
        static_format_image<float, 4>
    > imvar;
public:
};

}
