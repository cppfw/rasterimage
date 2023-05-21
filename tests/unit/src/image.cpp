#include <tst/check.hpp>
#include <tst/set.hpp>

#include <rasterimage/image.hpp>

namespace{
tst::set set("image", [](tst::suite& suite){
    suite.add("clear", [](){
        rasterimage::static_format_image<uint8_t, 4> im({100, 200});

        tst::check_eq(im.pixels().size(), size_t(100 * 200), SL);

        im.clear({10, 20, 30, 40});

        for(auto l : im){
            for(auto& p : l){
                tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
                tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
                tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
                tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
            }
        }

        for(uint32_t y = 0; y != im.dims().y(); ++y){
            for(uint32_t x = 0 ; x != im.dims().x(); ++x){
                const auto& p = im.pixels()[y * im.dims().x() + x];
                tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
                tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
                tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
                tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
            }
        }
    });

    suite.add("iterator_plus_equals__difference_type", [](){
        rasterimage::static_format_image<uint8_t, 4> im({100, 200});
        im.clear({10, 20, 30, 40});

        auto i = im.begin();
        auto j = i;

        i += 3;

        ++j;
        ++j;
        ++j;

        tst::check(i == j, SL);

        i += -2;

        --j;
        --j;
    });
});
}
