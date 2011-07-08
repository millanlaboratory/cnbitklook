toolbox = matndf
interface = $(toolbox)
matpath = ${pkgdatadir}/cnbitkmat/$(toolbox)
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m
ticc = $(top_srcdir)/mex/$(interface)/ndf_tic.c
tocc = $(top_srcdir)/mex/$(interface)/ndf_toc.c

EXTRA_DIST = ${matfiles}

all: mex

mex: mextic mextoc

mextic: $(ticc)
	mex $(ticc)

mextoc: $(tocc)
	mex $(tocc)

install-data-local:
	mkdir -p ${matpath}
	cp $(matfiles) ${matpath}
	cp *.mex* ${matpath}

uninstall-local:
	rm -rf ${matpath}
