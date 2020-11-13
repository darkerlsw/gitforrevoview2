PATH = lswtest

CREATEFILE := $(shell $(PERL) ./make/perl_script/gettime_tofile.pl build/$(PROJECT)_builddir/tmp/buildtime.h)

MINCPATH += build/$(PROJECT)_builddir/thread_test 

MINCPATH += $(PATH)/inc  


MSRCPATH += $(PATH)/src

SOURCES += thread_test.c