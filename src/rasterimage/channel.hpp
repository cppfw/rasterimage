#pragma once

namespace rasterimage{

// TODO: doxygen
template <typename value_type>
class channel{
    static_assert(
        std::is_same_v<value_type, uint8_t> ||
        std::is_same_v<value_type, uint16_t> ||
        std::is_same_v<value_type, float>
        , "only uint8_t, uint16_t or float are supported");
public:
    value_type value;

};

}
