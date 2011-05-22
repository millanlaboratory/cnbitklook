EXTRA_DIST = transport.mw.in

interface = transport
mexcc = $(interface).cc
mexmw = $(interface).mw
mexld = -l$(interface) 

cc:
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
	mex -largeArrayDims $(mexld) $(mexcc)

clean-local:
	rm -rf *.m
	rm -rf $(mexcc)
	rm -rf *.mex*
