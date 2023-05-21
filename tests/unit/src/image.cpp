#include <tst/check.hpp>
#include <tst/set.hpp>

#include <rasterimage/image.hpp>

namespace{
tst::set set("image", [](tst::suite& suite){
    suite.add("clear", [](){
        rasterimage::static_format_image<uint8_t, 4> im;

        im.clear({10, 20, 30, 40});

        for(auto l : im){
            for(auto& p : l){
                tst::check_eq(p.r(), decltype(im)::pixel_type::value_type(10), SL);
                tst::check_eq(p.g(), decltype(im)::pixel_type::value_type(20), SL);
                tst::check_eq(p.b(), decltype(im)::pixel_type::value_type(30), SL);
                tst::check_eq(p.a(), decltype(im)::pixel_type::value_type(40), SL);
            }
        }
    });
});
}
