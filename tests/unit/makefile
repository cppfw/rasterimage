include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_name := tests

this_srcs := $(call prorab-src-dir, src)

this_cxxflags += -I ../../src
this_ldlibs += -ltst

$(eval $(prorab-build-app))

this_test_cmd := $(prorab_this_name)
this_test_deps := $(prorab_space)
this_test_ld_path := ../../src/out/$(c)
$(eval $(prorab-test))
