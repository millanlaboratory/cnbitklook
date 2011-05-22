EXTRA_DIST = cnbiloop.mw.in

interface = cnbiloop
mexcc = $(interface).cc
mexmw = $(interface).mw
mexld = -l$(interface) -lcnbicore

all: mex

cc: $(mexmw)
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
	mex -largeArrayDims $(mexld) $(mexcc)

clean-local:
	rm -rf *.m
	rm -rf $(mexcc)
	rm -rf *.mex*
