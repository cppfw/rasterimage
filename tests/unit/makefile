include prorab.mk
include prorab-test.mk
include prorab-clang-format.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs := $(call prorab-src-dir, src)

this__lib_raster_image := ../../src/out/$(c)/librasterimage$(this_dbg)$(dot_so)

this_cxxflags += -isystem ../../src

this_ldlibs += $(this__lib_raster_image)

this_ldlibs += -l tst$(this_dbg)
this_ldlibs += -l utki$(this_dbg)

this_no_install := true

$(eval $(prorab-build-app))

$(eval $(prorab-clang-format))

this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_this_name)
this_test_ld_path := ../../src/out/$(c)
$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
