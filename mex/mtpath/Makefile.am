toolbox = mtpath
matpath = ${prefix}/opt/$(toolbox)/
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m

install-data-local:
	mkdir -p ${matpath}
	cp $(matfiles) ${matpath}

uninstall-local:
	rm -rf ${matpath}
