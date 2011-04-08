# 2011-01-22  Michele Tavella <michele.tavella@epfl.ch>
all: ndf_tic ndf_toc

ndf_tic: ndf_tic.c
	mex ndf_tic.c

ndf_toc: ndf_toc.c
	mex ndf_toc.c

clean:
	rm ndf_*.mex*

install:
	@cp -vf *.mex* ${CNBITK_MATLAB}/matndf/
	@cp -vf ndf_*.m ${CNBITK_MATLAB}/matndf/
