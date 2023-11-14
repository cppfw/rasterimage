#include <rasterimage/image.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
const tst::set set("image", [](tst::suite& suite) {
	suite.add("operator_square_brackets__size_t", []() {
		rasterimage::image<uint8_t, 4> im(rasterimage::dimensioned::dimensions_type{100, 200});
		im.span().clear({10, 20, 30, 40});

		decltype(im)::pixel_type expected = {0, 1, 2, 3};

		im[3][10] = expected;

		auto i = im.span().begin();

		tst::check_eq(i[3][10], expected, SL);
	});

	suite.add("make", []() {
		std::array<uint8_t, 4> data = {1, 2, 3, 4};

		constexpr auto stride = 2;

		auto im = rasterimage::image<uint8_t, 1>::make( //
			{2, 2},
			data.data(),
			stride
		);

		tst::check_eq(im[0][0].front(), data[0], SL);
		tst::check_eq(im[0][1].front(), data[1], SL);
		tst::check_eq(im[1][0].front(), data[2], SL);
		tst::check_eq(im[1][1].front(), data[3], SL);
	});
});
} // namespace
