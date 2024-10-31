/*
MIT License

Copyright (c) 2023-2024 Ivan Gagis <igagis@gmail.com>

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

#include <r4/rectangle.hpp>

#include "dimensioned.hpp"
#include "operations.hpp"

namespace rasterimage {
template <
	typename channel_type, //
	size_t number_of_channels>
class image;

template <
	typename channel_type, //
	size_t number_of_channels,
	bool is_const_span = false>
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
	size_t stride_px;

	std::conditional_t<is_const_span, const pixel_type, pixel_type>* buffer = nullptr;

	template <bool is_const>
	class iterator_internal
	{
		friend class image_span;

	public:
		using const_value_type = utki::span<const pixel_type>;

	private:
		using non_const_value_type = utki::span<pixel_type>;

		std::conditional_t<is_const, const_value_type, non_const_value_type> line;

		size_t stride_px{};

		iterator_internal(
			decltype(line) line, //
			size_t stride_px
		) :
			line(std::move(line)),
			stride_px(stride_px)
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
			this->line = utki::make_span(this->line.data() + this->stride_px, this->line.size());
			return *this;
		}

		iterator_internal& operator--() noexcept
		{
			this->line = utki::make_span(this->line.data() - this->stride_px, this->line.size());
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
			this->line = utki::make_span(this->line.data() + d * this->stride_px, this->line.size());

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
			ASSERT(this->stride_px == i.stride_px)
			if (this->line.data() >= i.line.data()) {
				return (this->line.data() - i.line.data()) / this->stride_px;
			} else {
				return -((i.line.data() - this->line.data()) / this->stride_px);
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

	image_span(
		dimensions_type dimensions, //
		size_t stride_px,
		pixel_type* buffer
	) :
		dimensioned(dimensions),
		stride_px(stride_px),
		buffer(buffer)
	{}

public:
	using iterator = iterator_internal<false>;
	using const_iterator = iterator_internal<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	image_span(image<channel_type, number_of_channels>& img);

	// TODO: add constructor to convert to const_image_span

	pixel_type* data()
	{
		return this->buffer;
	}

	const pixel_type* data() const
	{
		return this->buffer;
	}

	unsigned stride_pixels() const
	{
		return this->stride_px;
	}

	size_t stride_bytes() const
	{
		return this->stride_pixels() * sizeof(pixel_type);
	}

	bool empty() const noexcept
	{
		return this->buffer == nullptr;
	}

	iterator begin() noexcept
	{
		return iterator(utki::make_span(this->buffer, this->dims().x()), this->stride_px);
	}

	iterator end() noexcept
	{
		return utki::next(this->begin(), this->dims().y());
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator(utki::make_span(this->buffer, this->dims().x()), this->stride_px);
	}

	const_iterator cend() const noexcept
	{
		return utki::next(this->cbegin(), this->dims().y());
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

	image_span subspan(r4::rectangle<uint32_t> rect)
	{
		ASSERT(r4::rectangle<uint32_t>({0, 0}, this->dims()).contains(rect), [&](auto& o) {
			o << "requested subspan is out of the span, this->dims() = " << this->dims() << ", rect = " << rect;
		})

		image_span
			ret( //
				rect.d,
				this->stride_px,
				&(*this)[rect.p.y()][rect.p.x()]
			);

		return ret;
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

	void flip_vertical() noexcept
	{
		// NOTE: the std::prev(this->end()) cannot be used here because iterator_category is std::input_iterator_tag,
		//       while std::prev() requires at least std::bidirectional_iterator_tag.
		for (auto upper = this->begin(), lower = --this->end(); upper < lower; ++upper, --lower) {
			ASSERT(upper->size() == lower->size())

			for (auto u = upper->begin(), l = lower->begin(); u != upper->end(); ++u, ++l) {
				using std::swap;
				swap(*u, *l);
			}
		}
	}
};

template <
	typename channel_type, //
	size_t number_of_channels>
using const_image_span = image_span<channel_type, number_of_channels, true>;
} // namespace rasterimage
