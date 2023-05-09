#pragma once

#include <array>

#include <r4/vector.hpp>

namespace rasterimage{

enum class pixel_format{
    luminance,
    luminance_alpha,
    rgb,
    rgba
};

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

template <typename channel_type, pixel_format format>
class static_format_image{
    using pixel_type =
        std::conditional_t<format == pixel_format::luminance,
            channel_type,
        std::conditional_t<format == pixel_format::luminance_alpha,
            r4::vector2<channel_type>,
        std::conditional_t<format == pixel_format::rgb,
            r4::vector3<channel_type>,
        std::conditional_t<format == pixel_format::rgba,
            r4::vector4<channel_type>,
        std::enable_if_t<false, void>
        >>>>;

    std::vector<pixel_type> buffer;

    size_t stride;

public:

};

}
