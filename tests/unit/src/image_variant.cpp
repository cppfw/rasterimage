#include <rasterimage/image_variant.hpp>
#include <tst/check.hpp>
#include <tst/set.hpp>
#include <utki/enum_iterable.hpp>

namespace {
const tst::set set("image_variant", [](tst::suite& suite) {
	suite.add<std::tuple<rasterimage::format, rasterimage::depth, size_t>>(
		"image_num_channels",
		[]() {
			std::vector<std::tuple<rasterimage::format, rasterimage::depth, size_t>> ret;
			for (auto d : utki::enum_iterable_v<rasterimage::depth>) {
				for (auto f : utki::enum_iterable_v<rasterimage::format>) {
					ret.emplace_back(f, d, rasterimage::to_num_channels(f));
				}
			}
			return ret;
		}(),
		[](const auto& p) {
			rasterimage::image_variant im{
				{10, 20},
				std::get<rasterimage::format>(p),
				std::get<rasterimage::depth>(p)
			};

			tst::check_eq(im.num_channels(), std::get<size_t>(p), SL);
			tst::check(im.get_format() == std::get<rasterimage::format>(p), SL);
			tst::check(im.get_depth() == std::get<rasterimage::depth>(p), SL);

			rasterimage::image_variant empty_im{
				{0, 0},
				std::get<rasterimage::format>(p),
				std::get<rasterimage::depth>(p)
			};

			tst::check_eq(empty_im.num_channels(), std::get<size_t>(p), SL);
			tst::check(empty_im.get_format() == std::get<rasterimage::format>(p), SL);
			tst::check(empty_im.get_depth() == std::get<rasterimage::depth>(p), SL);
		}
	);

	suite.add("image_dims", []() {
		rasterimage::dimensioned::dimensions_type expected = {13, 20};

		for (auto d = 0; d != size_t(rasterimage::depth::enum_size); ++d) {
			for (auto f = 0; f != size_t(rasterimage::format::enum_size); ++f) {
				rasterimage::image_variant im{expected, rasterimage::format(f), rasterimage::depth(d)};

				tst::check_eq(im.dims(), expected, SL);
			}
		}
	});
});
} // namespace
