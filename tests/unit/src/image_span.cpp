#include <rasterimage/image.hpp>
#include <rasterimage/image_span.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
const tst::set set("image_span", [](tst::suite& suite) {
	suite.add("default_constructor", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		decltype(img)::image_span_type span;
		tst::check(span.empty(), SL);
		tst::check_eq(span.dims(), decltype(img)::dimensions_type(0), SL);

		span = img.span();
		span.clear(0);

		tst::check_eq(span[0][0], decltype(img)::pixel_type(0), SL);
	});

	suite.add("constructor_from_image", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});
		img.span().clear(10);

		decltype(img)::image_span_type im(img);
		tst::check_eq(im[0][0], decltype(im)::pixel_type{10, 10, 10, 10}, SL);
	});

	suite.add("constructor_from_const_image", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});
		img.span().clear(10);

		const auto& cimg = img;

		decltype(img)::const_image_span_type im(cimg);
		tst::check_eq(im[0][0], decltype(im)::pixel_type{10, 10, 10, 10}, SL);
	});

	suite.add("clear", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		for (auto l : im) {
			for (auto& p : l) {
				tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
				tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
				tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
				tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
			}
		}

		for (uint32_t y = 0; y != img.dims().y(); ++y) {
			for (uint32_t x = 0; x != img.dims().x(); ++x) {
				auto p = img.pixels()[y * img.dims().x() + x];
				tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
				tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
				tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
				tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
			}
		}
	});

	suite.add("iterator_operator_plus_equals__difference_type", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();
		auto j = i;

		i += 3;

		++j;
		j++;
		++j;

		tst::check(i == j, SL);

		i += -2;

		--j;
		j--;

		tst::check(i == j, SL);
	});

	suite.add("iterator_operator_minus_equals__difference_type", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();
		auto j = i;

		i += 3;

		i -= 3;

		tst::check(i == j, SL);

		i += -2;

		i -= -2;

		tst::check(i == j, SL);
	});

	suite.add("iterator_operator_plus__difference_type", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();
		auto j = i;

		i = i + 3;

		++j;
		j++;
		++j;

		tst::check(i == j, SL);

		i = i + (-2);

		--j;
		j--;

		tst::check(i == j, SL);
	});

	suite.add("iterator_operator_difference_type_plus_iterator", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();
		auto j = i;

		i = 3 + i;

		++j;
		j++;
		++j;

		tst::check(i == j, SL);

		i = -2 + i;

		--j;
		j--;

		tst::check(i == j, SL);
	});

	suite.add("iterator_operator_minus__difference_type", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();
		auto j = i;

		i += 3;

		i = i - 3;

		tst::check(i == j, SL);

		i += -2;

		i = i - (-2);

		tst::check(i == j, SL);
	});

	suite.add("iterator_operator_minus__iterator", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();
		auto j = i;

		j += 4;

		tst::check_eq(j - i, decltype(i)::difference_type(4), SL);
		tst::check_eq(i - j, decltype(i)::difference_type(-4), SL);
	});

	suite.add("iterator_operator_square_brackets__difference_type", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		decltype(im)::pixel_type expected = {0, 1, 2, 3};

		auto i = im.begin();
		img.pixels()[img.dims().x() * 2 + 3] = expected;

		tst::check_eq(i[2][3], expected, SL);

		i += 3;

		tst::check_eq(i[-1][3], {0, 1, 2, 3}, SL);
	});

	suite.add("iterator_operator_less_than__iterator", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();

		auto j = i + 3;

		tst::check(i < j, SL);
		tst::check(!(j < i), SL);
	});

	suite.add("iterator_operator_greater_than__iterator", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();

		auto j = i + 3;

		tst::check(j > i, SL);
		tst::check(!(i > j), SL);
	});

	suite.add("iterator_operator_greater_than_or_equals__iterator", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();

		auto j = i + 3;

		tst::check(j >= i, SL);
		tst::check(!(i >= j), SL);

		j = i;

		tst::check(i >= j, SL);
		tst::check(j >= i, SL);
	});

	suite.add("iterator_operator_less_than_or_equals__iterator", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		auto i = im.begin();

		auto j = i + 3;

		tst::check(i <= j, SL);
		tst::check(!(j <= i), SL);

		j = i;

		tst::check(i <= j, SL);
		tst::check(j <= i, SL);
	});

	suite.add("image_operator_square_brackets__size_t", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		im.clear({10, 20, 30, 40});

		decltype(im)::pixel_type expected = {0, 1, 2, 3};

		im[3][10] = expected;

		auto i = im.begin();

		tst::check_eq(i[3][10], expected, SL);
	});

	suite.add("image_cbegin_cend", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		decltype(im)::pixel_type expected = {10, 20, 30, 40};
		im.clear(expected);

		const auto& cim = im;

		auto b = cim.cbegin();
		auto e = cim.cend();

		tst::check_eq(b[0][0], expected, SL);

		b = utki::next(b, cim.dims().y());

		tst::check(b == e, SL);
	});

	suite.add("conversion_to_const_image_span", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();
		im.clear(0);

		decltype(im)::const_image_span_type cim = im;

		tst::check_eq(cim[0][0], decltype(cim)::pixel_type{0, 0, 0, 0}, SL);
	});

	suite.add("data", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();
		im.clear(0);

		decltype(im)::pixel_type expected1 = {10, 20, 30, 40};
		decltype(im)::pixel_type expected2 = {1, 2, 3, 4};

		auto subim = im.subspan({1, 2, 2, 3});
		subim.clear(expected1);

		subim.data()[0] = expected2; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		subim.data()[2] = expected2; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

		tst::check_eq(img[2][1], expected2, SL);
		tst::check_eq(img[2][2], expected1, SL);
		tst::check_eq(img[2][3], expected2, SL);
	});

	suite.add("stride", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();
		auto subim = im.subspan({1, 2, 2, 3});

		tst::check_eq(im.stride_pixels(), unsigned(100), SL);
		tst::check_eq(im.stride_bytes(), 100 * sizeof(decltype(img)::pixel_type), SL);
		tst::check_eq(subim.stride_pixels(), im.stride_pixels(), SL);
		tst::check_eq(subim.stride_bytes(), im.stride_bytes(), SL);
	});

	suite.add("subspan__rectangle", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		decltype(im)::pixel_type expected1 = {10, 20, 30, 40};
		im.clear(expected1);

		auto subim = im.subspan({1, 2, 2, 3});

		decltype(im)::pixel_type expected2(0);
		subim.clear(expected2);

		tst::check_eq(im[1][0], expected1, SL);
		tst::check_eq(im[1][1], expected1, SL);
		tst::check_eq(im[1][2], expected1, SL);
		tst::check_eq(im[1][3], expected1, SL);

		tst::check_eq(im[2][0], expected1, SL);
		tst::check_eq(im[2][1], expected2, SL);
		tst::check_eq(im[2][2], expected2, SL);
		tst::check_eq(im[2][3], expected1, SL);

		tst::check_eq(im[4][0], expected1, SL);
		tst::check_eq(im[4][1], expected2, SL);
		tst::check_eq(im[4][2], expected2, SL);
		tst::check_eq(im[4][3], expected1, SL);

		tst::check_eq(im[5][0], expected1, SL);
		tst::check_eq(im[5][1], expected1, SL);
		tst::check_eq(im[5][2], expected1, SL);
		tst::check_eq(im[5][3], expected1, SL);
	});

	suite.add("subspan__rectangle_const", []() {
		rasterimage::image<uint8_t, 4> img(rasterimage::dimensioned::dimensions_type{100, 200});

		auto im = img.span();

		decltype(im)::pixel_type expected1 = {10, 20, 30, 40};
		im.clear(expected1);

		auto subim = im.subspan({1, 2, 2, 3});

		decltype(im)::pixel_type expected2(0);
		subim.clear(expected2);

		const auto& cimg = img;
		auto cim = cimg.span();
		auto csubim = cim.subspan({1, 2, 2, 3});

		tst::check_eq(csubim[0][0], expected2, SL);
		tst::check_eq(csubim[0][1], expected2, SL);

		tst::check_eq(csubim[1][0], expected2, SL);
		tst::check_eq(csubim[1][1], expected2, SL);

		tst::check_eq(csubim[2][0], expected2, SL);
		tst::check_eq(csubim[2][1], expected2, SL);
	});

	suite.add<unsigned>("flip_vertical", {1, 2, 3, 4, 5, 10, 13}, [](const auto& p) {
		rasterimage::image<int, 4> img(rasterimage::dimensioned::dimensions_type{2, p});

		auto im = img.span();
		for (size_t i = 0; i != im.dims().y(); ++i) {
			int c = int(i * 10);
			im[i][0] = {c, c + 1, c + 2, c + 3};
			im[i][1] = {c, c + 4, c + 5, c + 6};
		}

		im.flip_vertical();

		for (size_t i = 0; i != im.dims().y(); ++i) {
			int c = int(im.dims().y() - i - 1) * 10;
			decltype(im)::pixel_type expected0 = {c, c + 1, c + 2, c + 3};
			decltype(im)::pixel_type expected1 = {c, c + 4, c + 5, c + 6};

			tst::check_eq(im[i][0], expected0, SL);
			tst::check_eq(im[i][1], expected1, SL);
		}
	});
});
} // namespace
