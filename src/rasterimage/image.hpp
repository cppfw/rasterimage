#pragma once

#include <variant>

#include "channel.hpp"
#include "static_format_image.hpp"

namespace rasterimage {

enum class depth {
	uint_8_bit,
	uint_16_bit,
	floating_point,

	enum_size
};

template <depth depth_enum>
using depth_type_t = std::conditional_t<
	depth_enum == depth::uint_8_bit,
	uint8_t,
	std::conditional_t<depth_enum == depth::uint_16_bit, uint16_t, float>>;

enum class format {
	grey,
	greya,
	rgb,
	rgba,

	enum_size
};

inline constexpr size_t to_num_channels(format f)
{
	return size_t(f) + 1;
}

// TODO: doxygen
class image
{
public:
	using variant_type = std::variant<
		static_format_image<channel<uint8_t>, 1>,
		static_format_image<channel<uint8_t>, 2>,
		static_format_image<channel<uint8_t>, 3>,
		static_format_image<channel<uint8_t>, 4>,
		static_format_image<channel<uint16_t>, 1>,
		static_format_image<channel<uint16_t>, 2>,
		static_format_image<channel<uint16_t>, 3>,
		static_format_image<channel<uint16_t>, 4>,
		static_format_image<channel<float>, 1>,
		static_format_image<channel<float>, 2>,
		static_format_image<channel<float>, 3>,
		static_format_image<channel<float>, 4>>;

private:
	variant_type variant;

	static size_t to_variant_index(format pixel_format, depth channel_depth);

public:
	image(
		const r4::vector2<uint32_t>& dimensions = {0, 0},
		format pixel_format = format::rgba,
		depth channel_depth = depth::uint_8_bit
	);

	size_t num_channels() const noexcept
	{
		auto ret = size_t(this->get_format()) + 1;
		ASSERT(
			std::visit(
				[](const auto& sfi) {
					return sfi.num_channels;
				},
				this->variant
			)
			== ret
		)
		return ret;
	}

	format get_format() const noexcept
	{
		auto ret = format(this->variant.index() % size_t(format::enum_size));
		ASSERT(
			format(std::visit(
				[](const auto& sfi) {
					return sfi.num_channels - 1;
				},
				this->variant
			))
			== ret
		)
		return ret;
	}

	depth get_depth() const noexcept
	{
		return depth(this->variant.index() / size_t(format::enum_size));
	}

	const dimensioned::dimensions_type& dims() const noexcept;

	template <format format_enum, depth depth_enum = depth::uint_8_bit>
	static_format_image<channel<depth_type_t<depth_enum>>, to_num_channels(format_enum)>& get()
	{
		return std::get<static_format_image<channel<depth_type_t<depth_enum>>, to_num_channels(format_enum)>>(
			this->variant
		);
	}

	template <format format_enum, depth depth_enum = depth::uint_8_bit>
	const static_format_image<channel<depth_type_t<depth_enum>>, to_num_channels(format_enum)>& get() const
	{
		return std::get<static_format_image<channel<depth_type_t<depth_enum>>, to_num_channels(format_enum)>>(
			this->variant
		);
	}
};

} // namespace rasterimage
