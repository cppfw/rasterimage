#pragma once

#include <variant>

#include "channel.hpp"
#include "static_format_image.hpp"

namespace rasterimage{

enum class depth{
    uint_8_bit,
    uint_16_bit,
    floating_point,

    enum_size
};

enum class format{
    grey,
    greya,
    rgb,
    rgba,

    enum_size
};

// TODO: doxygen
class image{
public:
    using variant_type = std::variant<
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
    >;
private:
    variant_type variant;

    static size_t to_variant_index(format pixel_format, depth channel_depth);
public:

    image(
        const r4::vector2<uint32_t>& dimensions = {0, 0},
        format pixel_format = format::rgba,
        depth channel_depth = depth::uint_8_bit
    );

    unsigned num_channels()const noexcept{
        return this->variant.index() % 4 + 1;
    }

    const r4::vector2<uint32_t>& dims()const noexcept;
};

}
