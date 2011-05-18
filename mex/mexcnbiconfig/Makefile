# 2010-03-17  Michele Tavella <michele.tavella@epfl.ch>
# Needed just if your environment is not properly configured
#INCPATH=-I/homes/mtavella/Build/include/ 
#LIBPATH=-L/homes/mtavella/Build/lib/ 

# Which library are we linking against?
PROJECT=cnbiconfig
LIBNAME=lib$(PROJECT)
LDSTUFF=-l$(PROJECT) -ltobiic -ltobiid -lcnbiconfig 
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
	@rm -vf ccfg_*.m
	@rm -vf ccfgtask_*.m
	@rm -vf ccfgtaskset_*.m
	@rm -vf core

local-install:
	@mkdir -p ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf *.mex* ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf ccfg_*.m ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf ccfgtask_*.m ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf ccfgtaskset_*.m ${CNBITKMAT_ROOT}/mex${PROJECT}/
	@cp -vf example_*.m ${CNBITKMAT_ROOT}/mex${PROJECT}/

install:
	@mkdir -p /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf *.mex* /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf ccfg_*.m /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf ccfgtask_*.m /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf ccfgtaskset_*.m /opt/cnbitkmat/mex${PROJECT}/
	@cp -vf example_*.m /opt/cnbitkmat/mex${PROJECT}/
