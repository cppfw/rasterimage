#include <rasterimage/image.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
const tst::set set("image", [](tst::suite& suite) {
	suite.add("image_operator_square_brackets__size_t", []() {
		rasterimage::image<uint8_t, 4> im(rasterimage::dimensioned::dimensions_type{100, 200});
		im.span().clear({10, 20, 30, 40});

		decltype(im)::pixel_type expected = {0, 1, 2, 3};

		im[3][10] = expected;

		auto i = im.span().begin();

		tst::check_eq(i[3][10], expected, SL);
	});
});
} // namespace
