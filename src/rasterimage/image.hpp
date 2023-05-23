#pragma once

#include <variant>

#include "channel.hpp"
#include "static_format_image.hpp"

namespace rasterimage{

enum class depth{
    uint_8_bit,
    uint_16_bit,
    floating_point
};

enum class pixel_format{
    alpha,
    luminance_alpha,
    rgb,
    rgba
};

// TODO: doxygen
class image{
    std::variant<
        static_format_image<channel<uint8_t>, 1>,
        static_format_image<channel<uint8_t>, 2>,
        static_format_image<channel<uint8_t>, 3>,
        static_format_image<channel<uint8_t>, 4>,
        static_format_image<channel<uint16_t>, 1>,
        static_format_image<channel<uint16_t>, 2>,
        static_format_image<channel<uint16_t>, 3>,
        static_format_image<channel<uint16_t>, 4>,
        static_format_image<channel<float>, 1>,
        static_format_image<channel<float>, 2>,
        static_format_image<channel<float>, 3>,
        static_format_image<channel<float>, 4>
    > imvar;
public:

    image(
        const r4::vector2<uint32_t>& dimensions = {0, 0},
        pixel_format pf = pixel_format::rgba,
        depth channel_depth = depth::uint_8_bit
    );

    unsigned num_channels()const noexcept{
        return this->imvar.index() % 4 + 1;
    }

    const r4::vector2<uint32_t>& dims()const noexcept;
};

}
