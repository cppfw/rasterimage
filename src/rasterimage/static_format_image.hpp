#pragma once

#include <utki/span.hpp>
#include <r4/vector.hpp>

namespace rasterimage{

template <typename channel_type, size_t num_channels>
class static_format_image{
    r4::vector2<uint32_t> dimensions{0, 0};
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

    class iterator{
        friend class static_format_image;

        utki::span<pixel_type> line;

        iterator(utki::span<pixel_type> line) :
            line(line)
        {}

    public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = int32_t;
        using value_type = decltype(line);
        using reference = value_type;

        iterator() = default;

        bool operator!=(const iterator& i)const noexcept{
            return this->line.data() != i.line.data();
        }

        bool operator==(const iterator& i)const noexcept{
            return this->line.data() == i.line.data();
        }

        utki::span<pixel_type> operator*()noexcept{
            return this->line;
        }

        utki::span<const pixel_type> operator*()const noexcept{
            return this->line;
        }

        utki::span<pixel_type>* operator->()noexcept{
            return &this->line;
        }

        const utki::span<const pixel_type>* operator->()const noexcept{
            return &this->line;
        }

        iterator& operator++()noexcept{
            this->line = utki::make_span(this->line.data() + this->line.size(), this->line.size());
            return *this;
        }

        iterator& operator--()noexcept{
            this->line = utki::make_span(this->line.data() - this->line.size(), this->line.size());
            return *this;
        }

        // postfix increment
        iterator operator++(int)noexcept{
            iterator ret(*this);
            this->operator++();
            return ret;
        }

        iterator& operator+=(difference_type d)noexcept{
            this->line = utki::make_span(
                this->line.data() + d * this->line.size(),
                this->line.size()
            );

            return *this;
        }
    };

    static_format_image() = default;

    static_format_image(r4::vector2<uint32_t> dimensions):
        dimensions(dimensions),
        buffer(this->dimensions.x() * this->dimensions.y())
    {
        
    }

    const decltype(dimensions)& dims()const noexcept{
        return this->dimensions;
    }

    iterator begin()noexcept{
        return iterator(utki::make_span(this->buffer.data(), this->dimensions.x()));
    }

    iterator end()noexcept{
        return iterator(utki::make_span(
            this->buffer.data() + this->dimensions.x() * this->dimensions.y(),
            0
        ));
    }

    void clear(pixel_type val){
        for(auto l : *this){
            for(auto& p : l){
                p = val;
            }
        }
    }

    utki::span<pixel_type> pixels()noexcept{
        return this->buffer;
    }
};

}
