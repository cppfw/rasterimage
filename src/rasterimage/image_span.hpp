/*
MIT License

Copyright (c) 2023 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#pragma once

#include "dimensioned.hpp"
#include "operations.hpp"

namespace rasterimage {
template <typename channel_type, size_t number_of_channels>
class image;

template <typename channel_type, size_t number_of_channels>
class image_span : public dimensioned
{
public:
	static const size_t num_channels = number_of_channels;

	using pixel_type = r4::vector<channel_type, num_channels>;
	using value_type = typename pixel_type::value_type;

	static_assert(sizeof(pixel_type) == sizeof(channel_type) * number_of_channels, "pixel_type has padding");

	static_assert(
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		sizeof(std::array<pixel_type, 2>) == sizeof(pixel_type) * 2,
		"pixel_type array has gaps"
	);

private:
	size_t stride;

	utki::span<pixel_type> span;

	template <bool is_const>
	class iterator_internal
	{
		friend class image_span;

	public:
		using const_value_type = utki::span<const pixel_type>;

	private:
		using non_const_value_type = utki::span<pixel_type>;

		std::conditional_t<is_const, const_value_type, non_const_value_type> line;

		size_t stride;

		iterator_internal(decltype(line) line, size_t stride) :
			line(std::move(line)),
			stride(stride)
		{}

	public:
		// The iterator cannot have stronger tag than std::input_iterator_tag
		// because it's reference type is value_type. Otherwise, the iterator is
		// actually a random access iterator.
		using iterator_category = std::input_iterator_tag;

		using difference_type = std::ptrdiff_t;
		using value_type = decltype(line);
		using reference = value_type;
		using pointer = void;

		iterator_internal() = default;

		bool operator!=(const iterator_internal& i) const noexcept
		{
			return this->line.data() != i.line.data();
		}

		bool operator==(const iterator_internal& i) const noexcept
		{
			return this->line.data() == i.line.data();
		}

		value_type operator*() noexcept
		{
			return this->line;
		}

		const_value_type operator*() const noexcept
		{
			return this->line;
		}

		const value_type* operator->() noexcept
		{
			return &this->line;
		}

		const const_value_type* operator->() const noexcept
		{
			return &this->line;
		}

		iterator_internal& operator++() noexcept
		{
			this->line = utki::make_span(this->line.data() + this->stride, this->line.size());
			return *this;
		}

		iterator_internal& operator--() noexcept
		{
			this->line = utki::make_span(this->line.data() - this->stride, this->line.size());
			return *this;
		}

		// postfix increment
		iterator_internal operator++(int) noexcept
		{
			iterator_internal ret(*this);
			this->operator++();
			return ret;
		}

		// postfix decrement
		iterator_internal operator--(int) noexcept
		{
			iterator_internal ret(*this);
			this->operator--();
			return ret;
		}

		iterator_internal& operator+=(difference_type d) noexcept
		{
			this->line = utki::make_span(this->line.data() + d * this->stride, this->line.size());

			return *this;
		}

		iterator_internal& operator-=(difference_type d) noexcept
		{
			return this->operator+=(-d);
		}

		iterator_internal operator+(difference_type d) const noexcept
		{
			iterator_internal ret = *this;
			ret += d;
			return ret;
		}

		friend iterator_internal operator+(difference_type d, const iterator_internal& i) noexcept
		{
			return i + d;
		}

		iterator_internal operator-(difference_type d) const noexcept
		{
			iterator_internal ret = *this;
			ret -= d;
			return ret;
		}

		difference_type operator-(const iterator_internal& i) const noexcept
		{
			ASSERT(!this->line.empty())
			if (this->line.data() >= i.line.data()) {
				return (this->line.data() - i.line.data()) / this->stride;
			} else {
				return -((i.line.data() - this->line.data()) / this->stride);
			}
		}

		value_type operator[](difference_type d) noexcept
		{
			return *(*this + d);
		}

		const_value_type operator[](difference_type d) const noexcept
		{
			return *(*this + d);
		}

		bool operator<(const iterator_internal& i) const noexcept
		{
			return this->line.data() < i.line.data();
		}

		bool operator>(const iterator_internal& i) const noexcept
		{
			return this->line.data() > i.line.data();
		}

		bool operator>=(const iterator_internal& i) const noexcept
		{
			return this->line.data() >= i.line.data();
		}

		bool operator<=(const iterator_internal& i) const noexcept
		{
			return this->line.data() <= i.line.data();
		}
	};

public:
	using iterator = iterator_internal<false>;
	using const_iterator = iterator_internal<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	image_span(image<channel_type, number_of_channels>& img);

	bool empty() const noexcept
	{
		return this->span.empty();
	}

	iterator begin() noexcept
	{
		return iterator(utki::make_span(this->span.data(), this->dimensions.x()), this->stride);
	}

	iterator end() noexcept
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		return iterator(
			utki::make_span(this->span.data() + this->stride * this->dimensions.y(), this->dimensions.x()),
			this->stride
		);
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator(utki::make_span(this->span.data(), this->dimensions.x()), this->stride);
	}

	const_iterator cend() const noexcept
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		return const_iterator(
			utki::make_span(this->span.data() + this->stride * this->dimensions.y(), this->dimensions.x()),
			this->stride
		);
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(this->cend());
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(this->cbegin());
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(this->end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(this->begin());
	}

	utki::span<pixel_type> operator[](uint32_t line_index) noexcept
	{
		return *utki::next(this->begin(), line_index);
	}

	utki::span<const pixel_type> operator[](uint32_t line_index) const noexcept
	{
		return *utki::next(this->begin(), line_index);
	}

	void clear(pixel_type val)
	{
		for (auto l : *this) {
			for (auto& p : l) {
				p = val;
			}
		}
	}

	void swap_red_blue() noexcept
	{
		using std::swap;
		for (auto l : *this) {
			for (auto& p : l) {
				swap(p.r(), p.b());
			}
		}
	}

	void unpremultiply_alpha() noexcept
	{
		for (auto l : *this) {
			for (auto& p : l) {
				p = rasterimage::unpremultiply_alpha(p);
			}
		}
	}
};
} // namespace rasterimage
