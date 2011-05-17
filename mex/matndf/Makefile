# 2011-01-22  Michele Tavella <michele.tavella@epfl.ch>
all: ndf_tic ndf_toc

ndf_tic: ndf_tic.c
	mex ndf_tic.c

ndf_toc: ndf_toc.c
	mex ndf_toc.c

clean:
	rm ndf_*.mex*

local-install:
	@mkdir -p ${CNBITKMAT_ROOT}/matndf/
	@cp -vf *.mex* ${CNBITKMAT_ROOT}/matndf/
	@cp -vf ndf_*.m ${CNBITKMAT_ROOT}/matndf/
	@cp -vf printf.m ${CNBITKMAT_ROOT}/matndf/

install:
	@mkdir -p /opt/cnbitkmat/matndf/
	@cp -vf *.mex* /opt/cnbitkmat/matndf/
	@cp -vf ndf_*.m /opt/cnbitkmat/matndf/
	@cp -vf printf.m /opt/cnbitkmat/matndf/
