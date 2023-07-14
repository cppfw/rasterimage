#include <rasterimage/operations.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
const tst::set set("operations", [](tst::suite& suite) {
	suite.add("to_32bit_pixel__uint8_t", []() {
		r4::vector4<uint8_t> px = {0xf1, 0xf2, 0xf3, 0xf4};

		auto pixel = rasterimage::to_32bit_pixel(px);

		tst::check_eq(pixel, uint32_t(0xf4f3f2f1), SL);
	});

	suite.add("from_32bit_pixel", []() {
		uint32_t pixel = 0xf4f3f2f1;

		auto px = rasterimage::from_32bit_pixel(pixel);

		tst::check_eq(px, r4::vector4<uint8_t>{0xf1, 0xf2, 0xf3, 0xf4}, SL);
	});

	suite.add("to_uint8_t__from_uint16_t", []() {
		r4::vector4<uint16_t> px = {0x1000, 0x2000, 0x3000, 0x4000};

		auto pixel = rasterimage::to<uint8_t>(px);

		tst::check_eq(pixel, r4::vector4<uint8_t>{0x0f, 0x1f, 0x2f, 0x3f}, SL) << std::hex << " pixel = 0x" << pixel;
	});

	suite.add("to_uint8_t__from_float", []() {
		r4::vector4<float> px = {0.125f, 0.25f, 0.5f, 0.75f};

		auto pixel = rasterimage::to<uint8_t>(px);

		tst::check_eq(pixel, r4::vector4<uint8_t>{0x1f, 0x3f, 0x7f, 0xbf}, SL) << std::hex << " pixel = 0x" << pixel;
	});

	suite.add("to_float__from_uint8_t", []() {
		r4::vector4<uint8_t> px{0x20, 0x40, 0x80, 0xc0};

		auto pixel = rasterimage::to<float>(px);

		constexpr auto eps = 0.005f;

		using std::abs;

		tst::check_le(abs(pixel.r() - 0.125f), eps, SL);
		tst::check_le(abs(pixel.g() - 0.25f), eps, SL);
		tst::check_le(abs(pixel.b() - 0.5f), eps, SL);
		tst::check_le(abs(pixel.a() - 0.75f), eps, SL);
	});
});
} // namespace
