#pragma once

#include <type_traits>
#include <cstdint>

namespace rasterimage{

// TODO: doxygen
template <typename underlying_value_type>
class channel{
public:
    using value_type = underlying_value_type;

    static_assert(
        std::is_same_v<value_type, uint8_t> ||
        std::is_same_v<value_type, uint16_t> ||
        std::is_same_v<value_type, float>
        , "only uint8_t, uint16_t or float are supported");

    value_type value;

};

}
