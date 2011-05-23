toolbox = mtpath
matpath = ${pkgdatadir}/cnbitkmat/$(toolbox)/
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m

install-data-local:
	mkdir -p ${matpath}
	cp $(matfiles) ${matpath}

uninstall-local:
	rm -rf ${matpath}
