#include <tst/check.hpp>
#include <tst/set.hpp>

#include <rasterimage/image.hpp>

namespace{
tst::set set("image", [](tst::suite& suite){
    suite.add("image_num_channels", [](){
        rasterimage::image im;
    });
});
}
