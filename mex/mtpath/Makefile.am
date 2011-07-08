toolbox = mtpath
matpath = ${DESTDIR}${pkgdatadir}/cnbitkmat/$(toolbox)/
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m

EXTRA_DIST = ${matfiles}

install-data-local:
	mkdir -p ${matpath}
	cp $(matfiles) ${matpath}

uninstall-local:
	rm -rf ${matpath}
