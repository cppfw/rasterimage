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

	suite.add("to_32bit_pixel__uint16_t", []() {
		r4::vector4<uint16_t> px = {0x1000, 0x2000, 0x3000, 0x4000};

		auto pixel = rasterimage::to_32bit_pixel(px);

		tst::check_eq(pixel, uint32_t(0x3f2f1f0f), SL) << std::hex << " pixel = 0x" << pixel;
	});

	suite.add("to_32bit_pixel__float", []() {
		r4::vector4<float> px = {0.125f, 0.25f, 0.5f, 0.75f};

		auto pixel = rasterimage::to_32bit_pixel(px);

		tst::check_eq(pixel, uint32_t(0xbf7f3f1f), SL) << std::hex << " pixel = 0x" << pixel;
	});
});
} // namespace
