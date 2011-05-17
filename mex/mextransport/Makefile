# 2010-03-17  Michele Tavella <michele.tavella@epfl.ch>
# Needed just if your environment is not properly configured
#INCPATH=-I/homes/mtavella/Build/include/ 
#LIBPATH=-L/homes/mtavella/Build/lib/ 

# Which library are we linking against?
PROJECT=transport
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
	@rm -vf tr_*.m
	@rm -vf core
	@rm -Rvf build/

local-install: 
	@mkdir -p ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf *.mex* ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf tr_* ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf example_* ${CNBITKMAT_ROOT}/mex${PROJECT}/

install: 
	@mkdir -p /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf *.mex* /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf tr_* /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf example_* /opt/cnbitkmat/mex${PROJECT}/
