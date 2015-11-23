toolbox = cnbitkmat
matpath = ${DESTDIR}${pkgdatadir}/$(toolbox)/
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m

EXTRA_DIST = ${matfiles}

install-data-local:
	mkdir -p ${matpath}
	cp $(matfiles) ${matpath}

uninstall-local:
	rm -rf ${matpath}
