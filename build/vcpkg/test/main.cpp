#include <rasterimage/image_variant.hpp>

#include <iostream>

int main(int argc, const char** argv){
    rasterimage::image_variant im({10, 20});

    std::cout << "im.dims() = " << im.dims() << std::endl;

    return 0;
}
