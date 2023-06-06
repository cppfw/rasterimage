#include <rasterimage/image.hpp>

int main(int argc, const char** argv){
	rasterimage::image<uint8_t, 4> im({100, 200});

	std::cout << "image dims = " << im.dims() << std::endl;

	return 0;
}
