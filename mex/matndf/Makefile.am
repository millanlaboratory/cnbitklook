toolbox = matndf
interface = $(toolbox)
matpath = ${prefix}/opt/cnbitkmat/$(toolbox)
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m
ticc = $(top_srcdir)/mex/$(interface)/ndf_tic.c
tocc = $(top_srcdir)/mex/$(interface)/ndf_toc.c

mex: mextic mextoc

mextic: $(ticc)
	mex $(ticc)

mextoc: $(tocc)
	mex $(tocc)

install-data-local: mex
	mkdir -p ${matpath}
	cp $(matfiles) ${matpath}
	cp *.mex* ${matpath}

uninstall-local:
	rm -rf ${matpath}
