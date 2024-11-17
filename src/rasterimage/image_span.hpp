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

	using const_image_span_type = image_span<channel_type, num_channels, true>;
	using pixel_type = std::conditional_t<
		is_const_span, //
		const r4::vector<channel_type, num_channels>,
		r4::vector<channel_type, num_channels>>;

	static_assert(sizeof(pixel_type) == sizeof(channel_type) * number_of_channels, "pixel_type has padding");

	static_assert(
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		sizeof(std::array<pixel_type, 2>) == sizeof(pixel_type) * 2,
		"pixel_type array has gaps"
	);

private:
	size_t stride_px = 0;

	pixel_type* buffer = nullptr;

	template <bool is_const>
	class iterator_internal
	{
		friend class image_span;

	public:
		using const_value_type = utki::span<const pixel_type>;

	private:
		using non_const_value_type = utki::span<pixel_type>;

		std::conditional_t<is_const, const_value_type, non_const_value_type> line;

		size_t stride_px = 0;

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

public:
	using iterator = iterator_internal<is_const_span>;
	using const_iterator = iterator_internal<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using value_type = typename iterator::value_type;

	explicit image_span(
		dimensions_type dimensions, //
		size_t stride_px,
		pixel_type* buffer
	) :
		dimensioned(dimensions),
		stride_px(stride_px),
		buffer(buffer)
	{}

	image_span() :
		dimensioned({0, 0})
	{}

	image_span(image<channel_type, number_of_channels>& img);

	/**
	 * @brief Conversion constructor from image.
	 * Constructor for automatic conversion to const_image_span or span of another convertible channel type.
	 */
	image_span(const image<channel_type, number_of_channels>& img);

	/**
	 * @brief Conversion constructor.
	 * Constructor for automatic conversion to const_image_span or span of another convertible channel type.
	 */
	template <typename other_channel_type, bool is_other_const_span>
	image_span(const image_span<other_channel_type, number_of_channels, is_other_const_span>& s) :
		image_span(
			s.dims(), //
			s.stride_pixels(),
			s.data()
		)
	{}

	pixel_type* data() noexcept
	{
		return this->buffer;
	}

	const pixel_type* data() const noexcept
	{
		return this->buffer;
	}

	unsigned stride_pixels() const noexcept
	{
		return unsigned(this->stride_px);
	}

	size_t stride_bytes() const noexcept
	{
		return this->stride_pixels() * sizeof(pixel_type);
	}

	bool empty() const noexcept
	{
		return this->buffer == nullptr;
	}

	/**
	 * @brief Get number of lines.
	 * @return Number of lines in this image span.
	 */
	size_t size() const noexcept
	{
		return this->dims().y();
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

	image_span subspan(r4::rectangle<uint32_t> rect) noexcept
	{
		ASSERT(r4::rectangle<uint32_t>({0, 0}, this->dims()).contains(rect), [&](auto& o) {
			o << "requested subspan is out of the span, this->dims() = " << this->dims() << ", rect = " << rect;
		})

		return image_span( //
				rect.d,
				this->stride_px,
				&(*this)[rect.p.y()][rect.p.x()]
			);
	}

	const_image_span_type subspan(r4::rectangle<uint32_t> rect) const noexcept
	{
		ASSERT(r4::rectangle<uint32_t>({0, 0}, this->dims()).contains(rect), [&](auto& o) {
			o << "requested subspan is out of the span, this->dims() = " << this->dims() << ", rect = " << rect;
		})

		return const_image_span_type( //
				rect.d,
				this->stride_px,
				&(*this)[rect.p.y()][rect.p.x()]
			);
	}

	void clear(pixel_type val) noexcept
	{
		static_assert(!is_const_span, "image_span is const, cannot clear");
		for (auto l : *this) {
			for (auto& p : l) {
				p = val;
			}
		}
	}

	void blit(
		const_image_span_type span, //
		r4::vector2<int> position
	)
	{
		static_assert(!is_const_span, "image_span is const, cannot blit to it");

		auto this_rect = r4::rectangle<int>(
			0, //
			this->dims().template to<int>()
		);

		auto span_rect_relative_to_this_rect = r4::rectangle<int>(
			position, //
			span.dims().template to<int>()
		);

		auto dst_rect = this_rect.intersect(span_rect_relative_to_this_rect);

		if (dst_rect.d.is_any_zero()) {
			// image to blit is out of destination span
			return;
		}

		ASSERT(dst_rect.p.is_positive_or_zero())
		ASSERT(dst_rect.d.is_positive())

		// rectangle on the source span which will actually be blitted
		auto src_rect = r4::rectangle<int>(max(-position, 0), dst_rect.d);

		ASSERT(src_rect.p.is_positive_or_zero())
		ASSERT(src_rect.d.is_positive())
		ASSERT(r4::rectangle<int>(0, span.dims().template to<int>()).contains(src_rect))

		auto dst_span = this->subspan(dst_rect.to<unsigned>());
		auto src_span = span.subspan(src_rect.to<unsigned>());

		ASSERT(!dst_span.empty())
		ASSERT(!src_span.empty())
		ASSERT(src_span.dims() == dst_span.dims())

		for (auto [s, d] : utki::views::zip(src_span, dst_span)) {
			std::copy(
				s.begin(), //
				s.end(),
				d.begin()
			);
		}
	}

	void swap_red_blue() noexcept
	{
		static_assert(!is_const_span, "image_span is const, cannot swap red and blue");

		using std::swap;
		for (auto l : *this) {
			for (auto& p : l) {
				swap(p.r(), p.b());
			}
		}
	}

	void unpremultiply_alpha() noexcept
	{
		static_assert(!is_const_span, "image_span is const, cannot unpremultiply alpha");
		for (auto l : *this) {
			for (auto& p : l) {
				p = rasterimage::unpremultiply_alpha(p);
			}
		}
	}

	void flip_vertical() noexcept
	{
		static_assert(!is_const_span, "image_span is const, cannot flip vetical");

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
