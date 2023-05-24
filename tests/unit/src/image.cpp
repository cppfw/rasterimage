#include <tst/check.hpp>
#include <tst/set.hpp>

#include <rasterimage/image.hpp>

namespace{
tst::set set("image", [](tst::suite& suite){
    suite.add<std::tuple<rasterimage::format, rasterimage::depth, size_t>>("image_num_channels",
    {
        {rasterimage::format::grey, rasterimage::depth::uint_8_bit, 1},
        {rasterimage::format::greya, rasterimage::depth::uint_8_bit, 2},
        {rasterimage::format::rgb, rasterimage::depth::uint_8_bit, 3},
        {rasterimage::format::rgba, rasterimage::depth::uint_8_bit, 4},
        {rasterimage::format::grey, rasterimage::depth::uint_16_bit, 1},
        {rasterimage::format::greya, rasterimage::depth::uint_16_bit, 2},
        {rasterimage::format::rgb, rasterimage::depth::uint_16_bit, 3},
        {rasterimage::format::rgba, rasterimage::depth::uint_16_bit, 4},
        {rasterimage::format::grey, rasterimage::depth::floating_point, 1},
        {rasterimage::format::greya, rasterimage::depth::floating_point, 2},
        {rasterimage::format::rgb, rasterimage::depth::floating_point, 3},
        {rasterimage::format::rgba, rasterimage::depth::floating_point, 4},
    },
    [](const auto& p){
        rasterimage::image im{
            {10, 20},
            std::get<rasterimage::format>(p),
            std::get<rasterimage::depth>(p)
        };

        tst::check_eq(im.num_channels(), std::get<size_t>(p), SL);
    });
});
}
