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

#include <variant>

#include <fsif/file.hpp>

#include "image.hpp"

namespace rasterimage {

/**
 * @brief Channel depth of the image.
 */
enum class depth {
	/**
	 * @brief 8-bit unsigned integer channel.
	 */
	uint_8_bit,

	/**
	 * @brief 16-bit unsigned integer channel.
	 */
	uint_16_bit,

	/**
	 * @brief 32-bit floating point channel.
	 */
	float_32_bit,

	enum_size
};

/**
 * @brief Channel value type for the given channel depth.
 * @tparam depth_enum - channel depth.
 */
template <depth depth_enum>
using depth_type_t = std::conditional_t<
	depth_enum == depth::uint_8_bit,
	uint8_t,
	std::conditional_t<
		depth_enum == depth::uint_16_bit,
		uint16_t,
		std::conditional_t<depth_enum == depth::float_32_bit, float, void>>>;

/**
 * @brief Pixel format of the image, i.e. number of components in a pixel.
 */
enum class format {
	/**
	 * @brief 1 component: grey.
	 */
	grey,

	/**
	 * @brief 1 component: grey.
	 */
	gray = grey,

	/**
	 * @brief 2 components: grey + alpha.
	 */
	greya,

	/**
	 * @brief 2 components: grey + alpha.
	 */
	graya = greya,

	/**
	 * @brief 3 components: red, green, blue.
	 */
	rgb,

	/**
	 * @brief 4 components: red, green, blue, alpha.
	 */
	rgba,

	enum_size
};

/**
 * @brief Get number of components in the given pixel format.
 * @param f - pixel format.
 * @return number of components in the given pixel format.
 */
inline constexpr size_t to_num_channels(format f)
{
	return size_t(f) + 1;
}

/**
 * @brief Get pixel format for the given number of components.
 * @param num_channels - number of components.
 * @return pixel format for the given number of components.
 */
inline constexpr format to_format(unsigned num_channels)
{
#ifdef DEBUG
	if (num_channels < 1 || 4 < num_channels) {
		throw std::logic_error("num_channels out of range");
	}
#endif
	return format(num_channels - 1);
}

/**
 * @brief A raster image with a variable pixel format and channel depth.
 * An image_variant can hold an image of any supported combination of pixel
 * format (number of channels) and channel depth.
 */
class image_variant
{
public:
	/**
	 * @brief Type of the underlying image variant.
	 */
	using variant_type = std::variant<
		image<uint8_t, 1>,
		image<uint8_t, 2>,
		image<uint8_t, 3>,
		image<uint8_t, 4>,
		image<uint16_t, 1>,
		image<uint16_t, 2>,
		image<uint16_t, 3>,
		image<uint16_t, 4>,
		image<float, 1>,
		image<float, 2>,
		image<float, 3>,
		image<float, 4>>;

	/**
	 * @brief The underlying image variant.
	 */
	variant_type variant;

private:
	/**
	 * @brief Convert pixel format and channel depth to variant index.
	 * @param pixel_format - pixel format.
	 * @param channel_depth - channel depth.
	 * @return variant index for the given pixel format and channel depth.
	 */
	static size_t to_variant_index(format pixel_format, depth channel_depth);

public:
	/**
	 * @brief Constructor.
	 * @param dimensions - image dimensions in pixels.
	 * @param pixel_format - pixel format, i.e. number of components in a pixel.
	 * @param channel_depth - channel depth.
	 */
	image_variant(
		const r4::vector2<uint32_t>& dimensions = {0, 0},
		format pixel_format = format::rgba,
		depth channel_depth = depth::uint_8_bit
	);

	/**
	 * @brief Constructor.
	 * @param im - image to move into the variant.
	 */
	template <typename channel_type, size_t num_channels>
	image_variant(image<channel_type, num_channels>&& im) :
		variant(std::move(im))
	{}

	/**
	 * @brief Get number of components in a pixel.
	 * @return number of components in a pixel.
	 */
	size_t num_channels() const noexcept
	{
		auto ret = size_t(this->get_format()) + 1;
		ASSERT(
			std::visit(
				[](const auto& sfi) {
					return sfi.num_channels;
				},
				this->variant
			) == ret
		)
		return ret;
	}

	/**
	 * @brief Get pixel format, i.e. number of components in a pixel.
	 * @return pixel format.
	 */
	format get_format() const noexcept
	{
		auto ret = format(this->variant.index() % size_t(rasterimage::format::enum_size));
		ASSERT(
			format(std::visit(
				[](const auto& sfi) {
					return sfi.num_channels - 1;
				},
				this->variant
			)) == ret
		)
		return ret;
	}

	/**
	 * @brief Get channel depth.
	 * @return channel depth.
	 */
	depth get_depth() const noexcept
	{
		auto v = this->variant.index() / size_t(rasterimage::format::enum_size);
		ASSERT(v < size_t(depth::enum_size))
		return depth(v);
	}

	/**
	 * @brief Get image dimensions in pixels.
	 * @return image dimensions in pixels.
	 */
	const dimensioned::dimensions_type& dims() const noexcept;

	/**
	 * @brief Check if the image is empty, i.e. has 0 by 0 dimensions.
	 * @return true if the image is empty.
	 */
	bool empty() const noexcept;

	/**
	 * @brief Get buffer size.
	 * @return Size of the underlying image buffer in pixels.
	 */
	size_t buffer_size() const noexcept;

	/**
	 * @brief Get a reference to the image held by the variant.
	 * The template parameters must match the held image's pixel format and channel depth,
	 * no conversion is performed.
	 * @tparam components_enum - pixel format of the held image.
	 * @tparam depth_enum - channel depth of the held image.
	 * @return a reference to the held image.
	 * @throw std::bad_variant_access if the variant does not hold an image
	 *         of the requested pixel format and channel depth.
	 */
	template <format components_enum, depth depth_enum = depth::uint_8_bit>
	image<depth_type_t<depth_enum>, to_num_channels(components_enum)>& get()
	{
		return std::get<image<depth_type_t<depth_enum>, to_num_channels(components_enum)>>(this->variant);
	}

	/**
	 * @brief Get a reference to the image held by the variant.
	 * The template parameters must match the held image's pixel format and channel depth,
	 * no conversion is performed.
	 * @tparam components_enum - pixel format of the held image.
	 * @tparam depth_enum - channel depth of the held image.
	 * @return a const reference to the held image.
	 * @throw std::bad_variant_access if the variant does not hold an image
	 *         of the requested pixel format and channel depth.
	 */
	template <format components_enum, depth depth_enum = depth::uint_8_bit>
	const image<depth_type_t<depth_enum>, to_num_channels(components_enum)>& get() const
	{
		return std::get<image<depth_type_t<depth_enum>, to_num_channels(components_enum)>>(this->variant);
	}

	/**
	 * @brief Write image to PNG file.
	 *
	 * @param fi - file interface for writing the file. Must not be opened.
	 *             Exisitng file will be overwritten.
	 */
	void write_png(const fsif::file& fi) const;
};

/**
 * @brief Read PNG image from file.
 * @param fi - file to read the image from. File must not be opened.
 * @return Image read from the file.
 */
image_variant read_png(const fsif::file& fi);

/**
 * @brief Read JPEG image from file.
 * @param fi - file to read the image from. File must not be opened.
 * @return Image read from the file.
 */
image_variant read_jpeg(const fsif::file& fi);

/**
 * @brief Read image from file.
 * Automatically detects the image file format by filename suffix.
 * @param fi - file to read the image from. File must not be opened.
 * @return Image read from file.
 */
image_variant read(const fsif::file& fi);

} // namespace rasterimage
