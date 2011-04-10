# 2010-03-17  Michele Tavella <michele.tavella@epfl.ch>
# Needed just if your environment is not properly configured
#INCPATH=-I/homes/mtavella/Build/include/ 
#LIBPATH=-L/homes/mtavella/Build/lib/ 

# Which library are we linking against?
PROJECT=cnbiloop
LIBNAME=lib$(PROJECT)
LDSTUFF=-l$(PROJECT) 
SOURCE_MW=$(PROJECT).mw
SOURCE_CC=$(PROJECT).cc

# Select platform
PLATFORM=$(shell echo -n `uname -s`)

all: $(PROJECT)

$(PROJECT): $(SOURCE_MW)
	@echo "Building and compiling the $(LIBNAME) MEX wrapper"
	@echo "Platform is: $(PLATFORM)"
	@echo "Generating wrapper file with MWrap"
	@mwrap -mex $(PROJECT) \
		-c $(SOURCE_CC) \
		-mb $(SOURCE_MW)
ifeq ($(PLATFORM), Linux)
	@echo "Calling Matlab MEX"
	@mex -largeArrayDims $(INCPATH) $(LIBPATH) $(LDSTUFF) $(SOURCE_CC)
else
	@echo "Please run make.m from Matlab/GNUMex"
endif

clean:
	@rm -vf *.mex*
	@rm -vf *.cc
	@rm -vf cl_*.m
	@rm -vf core

install:
	@cp -vf *.mex* ${CNBITK_MATLAB}/mexcnbiconfig/
	@cp -vf cl_*.m ${CNBITK_MATLAB}/mexcnbiconfig/
	@cp -vf example_*.m ${CNBITK_MATLAB}/mexcnbiconfig/
