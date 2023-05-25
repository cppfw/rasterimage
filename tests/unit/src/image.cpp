#include <rasterimage/image.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>

namespace {
tst::set set("image", [](tst::suite& suite) {
	suite.add<std::tuple<rasterimage::components, rasterimage::depth, size_t>>(
		"image_num_channels",
		{
			{ rasterimage::components::grey,     rasterimage::depth::uint_8_bit, 1},
			{rasterimage::components::greya,     rasterimage::depth::uint_8_bit, 2},
			{  rasterimage::components::rgb,     rasterimage::depth::uint_8_bit, 3},
			{ rasterimage::components::rgba,     rasterimage::depth::uint_8_bit, 4},
			{ rasterimage::components::grey,    rasterimage::depth::uint_16_bit, 1},
			{rasterimage::components::greya,    rasterimage::depth::uint_16_bit, 2},
			{  rasterimage::components::rgb,    rasterimage::depth::uint_16_bit, 3},
			{ rasterimage::components::rgba,    rasterimage::depth::uint_16_bit, 4},
			{ rasterimage::components::grey, rasterimage::depth::floating_point, 1},
			{rasterimage::components::greya, rasterimage::depth::floating_point, 2},
			{  rasterimage::components::rgb, rasterimage::depth::floating_point, 3},
			{ rasterimage::components::rgba, rasterimage::depth::floating_point, 4},
    },
		[](const auto& p) {
			rasterimage::image im{
				{10, 20},
				std::get<rasterimage::components>(p),
				std::get<rasterimage::depth>(p)
            };

			tst::check_eq(im.num_channels(), std::get<size_t>(p), SL);
			tst::check(im.get_components() == std::get<rasterimage::components>(p), SL);
			tst::check(im.get_depth() == std::get<rasterimage::depth>(p), SL);

			rasterimage::image empty_im{
				{0, 0},
				std::get<rasterimage::components>(p),
				std::get<rasterimage::depth>(p)
            };

			tst::check_eq(empty_im.num_channels(), std::get<size_t>(p), SL);
			tst::check(empty_im.get_components() == std::get<rasterimage::components>(p), SL);
			tst::check(empty_im.get_depth() == std::get<rasterimage::depth>(p), SL);
		}
	);

	suite.add("image_dims", []() {
		rasterimage::dimensioned::dimensions_type expected = {13, 20};

		for (auto d = 0; d != size_t(rasterimage::depth::enum_size); ++d) {
			for (auto f = 0; f != size_t(rasterimage::components::enum_size); ++f) {
				rasterimage::image im{expected, rasterimage::components(f), rasterimage::depth(d)};

				tst::check_eq(im.dims(), expected, SL);
			}
		}
	});
});
} // namespace
