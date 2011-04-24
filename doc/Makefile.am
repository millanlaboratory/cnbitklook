EXTRA_DIST = doxygen.cfg.in
SUBDIRS = examples/ndf \
          examples/transport \
          examples/transpipe \
          examples/cnbicore \
          examples/cnbiconfig \
          examples/cnbiloop

html: html/index.html

html/index.html:
	doxygen $(builddir)/doxygen.cfg

install-html-local: html/index.html
	cp -r html $(htmldir)

uninstall-local:
	rm -rf $(htmldir)

clean-local:
	rm -rf html

