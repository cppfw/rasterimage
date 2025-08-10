#include <rasterimage/image.hpp>

#include <iostream>

int main(int argc, const char** argv){
	rasterimage::image<uint8_t, 4> im(rasterimage::dimensioned::dimensions_type{100, 200});

	std::cout << "image dims = " << im.dims() << std::endl;

	return 0;
}
