EXTRA_DIST = cnbiconfig.mw.in

interface = cnbiconfig
mexcc = $(interface).cc
mexmw = $(interface).mw
mexld = -l$(interface) -ltobiic -ltobiid -lcnbiconfig

all: mex

cc:
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
	mex -largeArrayDims $(mexld) $(mexcc)

clean-local:
	rm -rf *.m
	rm -rf $(mexcc)
	rm -rf *.mex*
