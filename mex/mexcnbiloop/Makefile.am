interface = cnbiloop
mexcc = $(interface).cc
mexmw = $(top_srcdir)/mex/mex$(interface)/$(interface).mw
mexld = -l$(interface) -lcnbicore
mexpath = ${prefix}/opt/cnbitkmat/mex$(interface)/

cc: $(mexmw)
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
	mex -largeArrayDims $(mexld) $(mexcc)

clean-local:
	rm -rf $(mexcc)
	rm -rf *.m
	rm -rf *.mex*

install-data-local: mex
	mkdir -p ${mexpath}
	cp *.m ${mexpath}
	cp *.mex* ${mexpath}

uninstall-local:
	rm -rf ${mexpath}
