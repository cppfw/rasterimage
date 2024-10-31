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

#include <r4/vector.hpp>
#include <utki/debug.hpp>
#include <utki/span.hpp>

#include "dimensioned.hpp"
#include "image_span.hpp"
#include "operations.hpp"

// TODO: doxygen
namespace rasterimage {

template <typename channel_type, size_t number_of_channels>
class image : public dimensioned
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
	std::vector<pixel_type> buffer;

public:
	image() :
		image(dimensions_type{0, 0})
	{}

	image(dimensions_type dimensions) :
		dimensioned(dimensions),
		buffer(dimensions.x() * dimensions.y())
	{}

	image(
		dimensions_type dimensions, //
		pixel_type fill
	) :
		image(dimensions)
	{
		std::fill(this->pixels().begin(), this->pixels().end(), fill);
	}

	image(
		dimensions_type dimensions, //
		decltype(buffer) buffer
	) :
		dimensioned(dimensions),
		buffer(std::move(buffer))
	{
		ASSERT(this->dims().x() * this->dims().y() == this->pixels().size(), [this](auto& o) {
			o << "rasterimage::image::image(dims, buffer): dimensions do not match with pixels array size"
			  << "\n";
			o << "\t"
			  << "dims = " << this->dims() << ", pixels().size() = " << this->pixels().size();
		});
	}

	image_span<channel_type, number_of_channels> span() noexcept
	{
		return *this;
	}

	image_span<const channel_type, number_of_channels> span() const noexcept
	{
		return *this;
	}

	bool empty() const noexcept
	{
		return this->buffer.empty();
	}

	utki::span<pixel_type> pixels() noexcept
	{
		return this->buffer;
	}

	utki::span<const pixel_type> pixels() const noexcept
	{
		return this->buffer;
	}

	utki::span<pixel_type> operator[](uint32_t line_index) noexcept
	{
		return utki::make_span(&this->buffer[this->dims().x() * line_index], this->dims().x());
	}

	utki::span<const pixel_type> operator[](uint32_t line_index) const noexcept
	{
		return utki::make_span(&this->buffer[this->dims().x() * line_index], this->dims().x());
	}

	static image make(
		dimensions_type dims, //
		const value_type* data,
		size_t stride_in_values = 0
	)
	{
		if (stride_in_values == 0) {
			stride_in_values = dims.x() * num_channels;
		}

		image im(dims);

		auto num_values_per_row = im.dims().x() * num_channels;

		auto src_row = data;

		for (auto row : im.span()) {
			ASSERT(row.size() * num_channels == num_values_per_row)
			std::copy( //
				src_row,
				// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
				src_row + num_values_per_row,
				// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
				reinterpret_cast<value_type*>(row.data())
			);
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			src_row += stride_in_values;
		}

		ASSERT(src_row == data + stride_in_values * im.dims().y())

		return im;
	}

	constexpr static value_type value(float f)
	{
		return rasterimage::value<value_type>(f);
	}
};

template <typename channel_type, size_t number_of_channels, bool is_const_span>
image_span<channel_type, number_of_channels, is_const_span>::image_span(image<channel_type, number_of_channels>& im) :
	dimensioned(im.dims()),
	stride_px(im.dims().x()),
	buffer(&im.pixels().front())
{}

} // namespace rasterimage
