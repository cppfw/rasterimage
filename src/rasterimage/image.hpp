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

    image(depth channel_depth, unsigned num_chans);

    unsigned num_channels()const noexcept{
        return this->imvar.index() % 4 + 1;
    }
};

}
