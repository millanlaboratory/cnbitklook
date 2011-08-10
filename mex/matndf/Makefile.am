toolbox = matndf
interface = ${toolbox}
matpath = ${DESTDIR}${pkgdatadir}/cnbitkmat/${toolbox}
matfiles = ${top_srcdir}/mex/${toolbox}/*.m
ticc = ${top_srcdir}/mex/${interface}/ndf_tic.c
tocc = ${top_srcdir}/mex/${interface}/ndf_toc.c

EXTRA_DIST = ${matfiles} ${ticc} ${tocc}

all: tic toc

tic: ${ticc}
	mex ${ticc}

toc: ${tocc}
	mex ${tocc}

install-data-local:
	mkdir -p ${matpath}
	cp ${matfiles} ${matpath}
	cp *.mex* ${matpath}

uninstall-local:
	rm -rf ${matpath}
