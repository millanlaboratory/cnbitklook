interface = cnbiloop
mexcc = $(interface).cc
mexmw = $(top_srcdir)/mex/mex$(interface)/$(interface).mw
mexld = -L$(abs_top_builddir)/src/$(interface)/.libs/ -l$(interface) \
		-L$(abs_top_builddir)/src/cnbicore/.libs/ -lcnbicore
mexinc = -I$(top_srcdir)/src/
mexpath = ${pkgdatadir}/cnbitkmat/mex$(interface)/

EXTRA_DIST = ${mexmw}

all: mex

$(interface).cc: $(mexmw)
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: $(interface).cc
	mex -largeArrayDims $(mexinc) $(mexcc) $(mexld)

clean-local:
	rm -rf $(mexcc)
	rm -rf *.m
	rm -rf *.mex*

install-data-local:
	mkdir -p ${mexpath}
	cp *.m ${mexpath}
	cp *.mex* ${mexpath}

uninstall-local:
	rm -rf ${mexpath}
