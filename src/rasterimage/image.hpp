/*
MIT License

Copyright (c) 2023-2026 Ivan Gagis <igagis@gmail.com>

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

namespace rasterimage {

/**
 * @brief A raster image.
 * A raster image is a rectangular array of pixels, where each pixel is a
 * vector of channel values.
 * @tparam channel_type - type of a single pixel channel value.
 * @tparam number_of_channels - number of channels in a pixel.
 */
template <typename channel_type, size_t number_of_channels>
class image : public dimensioned
{
public:
	/**
	 * @brief Number of channels in a pixel.
	 */
	static const size_t num_channels = number_of_channels;

	/**
	 * @brief Type of the image span.
	 */
	using image_span_type = image_span<channel_type, num_channels>;

	/**
	 * @brief Type of the const image span.
	 */
	using const_image_span_type = const_image_span<channel_type, num_channels>;

	/**
	 * @brief Type of a pixel, a vector of channel values.
	 */
	using pixel_type = r4::vector<channel_type, num_channels>;

	static_assert(sizeof(pixel_type) == sizeof(channel_type) * number_of_channels, "pixel_type has padding");

	static_assert(
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		sizeof(std::array<pixel_type, 2>) == sizeof(pixel_type) * 2,
		"pixel_type array has gaps"
	);

private:
	std::vector<pixel_type> buffer;

public:
	/**
	 * @brief Default constructor.
	 * Creates an empty image with 0 by 0 dimensions.
	 */
	image() :
		image(dimensions_type{0, 0})
	{}

	/**
	 * @brief Constructor.
	 * All pixels are initialized to zero.
	 * @param dimensions - image dimensions in pixels.
	 */
	image(dimensions_type dimensions) :
		dimensioned(dimensions),
		buffer(dimensions.x() * dimensions.y())
	{}

	/**
	 * @brief Constructor.
	 * All pixels are set to the given fill value.
	 * @param dimensions - image dimensions in pixels.
	 * @param fill - pixel value to fill the image with.
	 */
	image(
		dimensions_type dimensions, //
		pixel_type fill
	) :
		image(dimensions)
	{
		std::fill(this->pixels().begin(), this->pixels().end(), fill);
	}

	/**
	 * @brief Constructor.
	 * Takes ownership of the given pre-allocated pixel buffer.
	 * The buffer must contain exactly dims.x() * dims.y() pixels,
	 * otherwise the behavior is undefined (asserted in debug builds).
	 * @param dimensions - image dimensions in pixels.
	 * @param buffer - pre-allocated pixel buffer to take ownership of.
	 */
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

	/**
	 * @brief Get a span representing the whole image.
	 * @return span of the image pixels.
	 */
	image_span_type span() noexcept
	{
		return *this;
	}

	/**
	 * @brief Get a span representing the whole image.
	 * @return span of the image pixels.
	 */
	const_image_span_type span() const noexcept
	{
		return const_image_span_type(*this);
	}

	/**
	 * @brief Check if the image is empty, i.e. has 0 by 0 dimensions.
	 * @return true if the image is empty.
	 */
	bool empty() const noexcept
	{
		return this->buffer.empty();
	}

	/**
	 * @brief Get a span of the image pixels.
	 * The pixels are stored densely, line by line.
	 * @return span of the image pixels.
	 */
	utki::span<pixel_type> pixels() noexcept
	{
		return this->buffer;
	}

	/**
	 * @brief Get a span of the image pixels.
	 * The pixels are stored densely, line by line.
	 * @return span of the image pixels.
	 */
	utki::span<const pixel_type> pixels() const noexcept
	{
		return this->buffer;
	}

	/**
	 * @brief Get a span of the pixels of a single image line.
	 * @param line_index - index of the line.
	 * @return span of the pixels of the given line.
	 */
	utki::span<pixel_type> operator[](uint32_t line_index) noexcept
	{
		return utki::make_span(&this->buffer[this->dims().x() * line_index], this->dims().x());
	}

	/**
	 * @brief Get a span of the pixels of a single image line.
	 * @param line_index - index of the line.
	 * @return span of the pixels of the given line.
	 */
	utki::span<const pixel_type> operator[](uint32_t line_index) const noexcept
	{
		return utki::make_span(&this->buffer[this->dims().x() * line_index], this->dims().x());
	}

	/**
	 * @brief Create an image from a buffer of channel values.
	 * The buffer data is packed line by line, with the stride in channel values
	 * between the starts of consecutive lines given by stride_in_values.
	 * @param dims - image dimensions in pixels.
	 * @param data - buffer of channel values. The buffer must contain at least
	 *               stride_in_values * dims.y() channel values (or dims.x() * num_channels * dims.y()
	 *               if stride_in_values is 0).
	 * @param stride_in_values - number of channel values between the start of
	 * consecutive lines in the buffer, 0 to assume the buffer is densely packed.
	 * @return newly created image.
	 */
	static image make(
		dimensions_type dims, //
		const typename pixel_type::value_type* data,
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
				reinterpret_cast<typename pixel_type::value_type*>(row.data())
			);
			// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
			src_row += stride_in_values;
		}

		ASSERT(src_row == data + stride_in_values * im.dims().y())

		return im;
	}

	/**
	 * @brief Convert a normalized value to a channel value.
	 * @param f - normalized value in [0:1] range.
	 * @return corresponding channel value.
	 */
	constexpr static typename pixel_type::value_type value(float f)
	{
		return rasterimage::value<typename pixel_type::value_type>(f);
	}
};

template <typename channel_type, size_t number_of_channels, bool is_const_span>
image_span<channel_type, number_of_channels, is_const_span>::image_span(image<channel_type, number_of_channels>& im) :
	dimensioned(im.dims()),
	stride_px(im.dims().x()),
	buffer(im.pixels().data())
{}

template <typename channel_type, size_t number_of_channels, bool is_const_span>
image_span<channel_type, number_of_channels, is_const_span>::image_span(
	const image<channel_type, number_of_channels>& im
) :
	dimensioned(im.dims()),
	stride_px(im.dims().x()),
	buffer(im.pixels().data())
{}

} // namespace rasterimage
