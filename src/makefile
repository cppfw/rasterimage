include prorab.mk
include prorab-clang-format.mk
include prorab-install-dbgsrc.mk
include prorab-license.mk

$(eval $(call prorab-config, ../config))

this_src_dir := rasterimage

this_name := $(this_src_dir)$(this_dbg)

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

this_ldlibs += -l fsif$(this_dbg)
this_ldlibs += -l png
this_ldlibs += -l jpeg

$(eval $(prorab-build-lib))

this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))
