include prorab.mk
include prorab-clang-format.mk

$(eval $(call prorab-config, ../config))

this_name := rasterimage

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

$(eval $(prorab-build-lib))

$(eval $(prorab-clang-format))
