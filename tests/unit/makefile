include prorab.mk
include prorab-test.mk
include prorab-clang-format.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs := $(call prorab-src-dir, src)

this_lib_raster_image := ../../src/out/$(c)/librasterimage$(dot_so)

this_cxxflags += -isystem ../../src
this_ldlibs += $(this_lib_raster_image)
this_ldlibs += -ltst -lutki

this_no_install := true

$(eval $(prorab-build-app))

$(eval $(prorab-clang-format))

$(eval $(call prorab-depend, $(prorab_this_name), $(this_lib_raster_image)))

this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)
$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
