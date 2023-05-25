#include <rasterimage/static_format_image.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
tst::set set("static_format_image", [](tst::suite &suite) {
  suite.add("clear", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});

    tst::check_eq(im.pixels().size(), size_t(100 * 200), SL);

    im.clear({10, 20, 30, 40});

    for (auto l : im) {
      for (auto &p : l) {
        tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
        tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
        tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
        tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
      }
    }

    for (uint32_t y = 0; y != im.dims().y(); ++y) {
      for (uint32_t x = 0; x != im.dims().x(); ++x) {
        const auto &p = im.pixels()[y * im.dims().x() + x];
        tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
        tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
        tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
        tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
      }
    }
  });

  suite.add("iterator_operator_plus_equals__difference_type", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
    im.clear({10, 20, 30, 40});

    auto i = im.begin();
    auto j = i;

    j += 4;

    tst::check_eq(j - i, decltype(i)::difference_type(4), SL);
    tst::check_eq(i - j, decltype(i)::difference_type(-4), SL);
  });

  suite.add("iterator_operator_square_brackets__difference_type", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
    im.clear({10, 20, 30, 40});

    decltype(im)::pixel_type expected = {0, 1, 2, 3};

    auto i = im.begin();
    im.pixels()[im.dims().x() * 2 + 3] = expected;

    tst::check_eq(i[2][3], expected, SL);

    i += 3;

    tst::check_eq(i[-1][3], {0, 1, 2, 3}, SL);
  });

  suite.add("iterator_operator_less_than__iterator", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
    im.clear({10, 20, 30, 40});

    auto i = im.begin();

    auto j = i + 3;

    tst::check(i < j, SL);
    tst::check(!(j < i), SL);
  });

  suite.add("iterator_operator_greater_than__iterator", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
    im.clear({10, 20, 30, 40});

    auto i = im.begin();

    auto j = i + 3;

    tst::check(j > i, SL);
    tst::check(!(i > j), SL);
  });

  suite.add("iterator_operator_greater_than_or_equals__iterator", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
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
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
    im.clear({10, 20, 30, 40});

    decltype(im)::pixel_type expected = {0, 1, 2, 3};

    im[3][10] = expected;

    auto i = im.begin();

    tst::check_eq(i[3][10], expected, SL);
  });

  suite.add("image_cbegin_cend", []() {
    rasterimage::static_format_image<uint8_t, 4> im({100, 200});
    decltype(im)::pixel_type expected = {10, 20, 30, 40};
    im.clear(expected);

    const auto &cim = im;

    auto b = cim.cbegin();
    auto e = cim.cend();

    tst::check_eq(b[0][0], expected, SL);

    b += cim.dims().y();

    tst::check(b == e, SL);
  });
});
} // namespace
