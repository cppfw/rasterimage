#pragma once

#include <variant>

#include "channel.hpp"
#include "static_format_image.hpp"

namespace rasterimage{

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

    unsigned num_channels()const noexcept;
};

}
