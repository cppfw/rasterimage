#pragma once

#include <utki/debug.hpp>
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
        // The iterator cannot have stronger tag than std::input_iterator_tag because
        // it's reference type is value_type. Otherwise, the iterator is actually
        // a random access iterator.
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

        // postfix decrement
        iterator operator--(int)noexcept{
            iterator ret(*this);
            this->operator--();
            return ret;
        }

        iterator& operator+=(difference_type d)noexcept{
            this->line = utki::make_span(
                this->line.data() + d * this->line.size(),
                this->line.size()
            );

            return *this;
        }

        iterator& operator-=(difference_type d)noexcept{
            return this->operator+=(-d);
        }

        iterator operator+(difference_type d)const noexcept{
            iterator ret = *this;
            ret += d;
            return ret;
        }

        friend iterator operator+(difference_type d, const iterator& i)noexcept{
            return i + d;
        }

        iterator operator-(difference_type d)const noexcept{
            iterator ret = *this;
            ret -= d;
            return ret;
        }

        difference_type operator-(const iterator& i)const noexcept{
            ASSERT(!this->line.empty())
            if(this->line.data() >= i.line.data()){
                return (this->line.data() - i.line.data()) / this->line.size();
            }else{
                return -((i.line.data() - this->line.data()) / this->line.size());
            }
        }

        utki::span<pixel_type> operator[](difference_type d)const noexcept{
            return *(*this + d);
        }

        bool operator<(const iterator& i)const noexcept{
            return this->line.data() < i.line.data();
        }

        bool operator>(const iterator& i)const noexcept{
            return this->line.data() > i.line.data();
        }

        bool operator>=(const iterator& i)const noexcept{
            return this->line.data() >= i.line.data();
        }

        bool operator<=(const iterator& i)const noexcept{
            return this->line.data() <= i.line.data();
        }
    };

    static_format_image() = default;

    static_format_image(r4::vector2<uint32_t> dimensions):
        dimensions(dimensions),
        buffer(this->dimensions.x() * this->dimensions.y())
    {
        ASSERT(!this->buffer.empty())
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

    utki::span<pixel_type> operator[](size_t line_index)noexcept{
        return *utki::next(this->begin(), line_index);
    }
};

}
