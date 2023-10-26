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

	suite.add("get_rgba__from_uint8_t", []() {
		r4::vector<uint8_t, 1> g = 0x20;
		r4::vector2<uint8_t> ga = {0x20, 0x30};
		r4::vector3<uint8_t> rgb = {0x20, 0x30, 0x40};
		r4::vector4<uint8_t> rgba = {0x20, 0x30, 0x40, 0x50};

		tst::check_eq(rasterimage::get_rgba(g), r4::vector4<uint8_t>{0x20, 0x20, 0x20, 0xff}, SL);
		tst::check_eq(rasterimage::get_rgba(ga), r4::vector4<uint8_t>{0x20, 0x20, 0x20, 0x30}, SL);
		tst::check_eq(rasterimage::get_rgba(rgb), r4::vector4<uint8_t>{0x20, 0x30, 0x40, 0xff}, SL);
		tst::check_eq(rasterimage::get_rgba(rgba), r4::vector4<uint8_t>{0x20, 0x30, 0x40, 0x50}, SL);
	});

	suite.add("get_rgba__from_float", []() {
		r4::vector<float, 1> g = {0.1f};
		r4::vector2<float> ga = {0.1f, 0.2f};
		r4::vector3<float> rgb = {0.1f, 0.2f, 0.3f};
		r4::vector4<float> rgba = {0.1f, 0.2f, 0.3f, 0.4f};

		tst::check_eq(rasterimage::get_rgba(g), r4::vector4<float>{0.1f, 0.1f, 0.1, 1}, SL);
		tst::check_eq(rasterimage::get_rgba(ga), r4::vector4<float>{0.1f, 0.1f, 0.1f, 0.2f}, SL);
		tst::check_eq(rasterimage::get_rgba(rgb), r4::vector4<float>{0.1f, 0.2f, 0.3f, 1}, SL);
		tst::check_eq(rasterimage::get_rgba(rgba), r4::vector4<float>{0.1f, 0.2f, 0.3f, 0.4f}, SL);
	});

	suite.add("get_alpha__from_uint8_t", []() {
		r4::vector<uint8_t, 1> g = 0x20;
		r4::vector2<uint8_t> ga = {0x20, 0x30};
		r4::vector3<uint8_t> rgb = {0x20, 0x30, 0x40};
		r4::vector4<uint8_t> rgba = {0x20, 0x30, 0x40, 0x50};

		tst::check_eq(rasterimage::get_alpha(g), uint8_t(0x20), SL);
		tst::check_eq(rasterimage::get_alpha(ga), uint8_t(0x30), SL);
		tst::check_eq(rasterimage::get_alpha(rgb), uint8_t(0xff), SL);
		tst::check_eq(rasterimage::get_alpha(rgba), uint8_t(0x50), SL);
	});

	suite.add("get_alpha__from_float", []() {
		r4::vector<float, 1> a = {0.1f};
		r4::vector2<float> ga = {0.1f, 0.2f};
		r4::vector3<float> rgb = {0.1f, 0.2f, 0.3f};
		r4::vector4<float> rgba = {0.1f, 0.2f, 0.3f, 0.4f};

		tst::check_eq(rasterimage::get_alpha(a), 0.1f, SL);
		tst::check_eq(rasterimage::get_alpha(ga), 0.2f, SL);
		tst::check_eq(rasterimage::get_alpha(rgb), 1.0f, SL);
		tst::check_eq(rasterimage::get_alpha(rgba), 0.4f, SL);
	});
});
} // namespace
