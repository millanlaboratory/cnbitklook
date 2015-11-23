EXTRA_DIST = doxygen.cfg.in
SUBDIRS = examples/cnbiconfig \
		  examples/cnbicore \
		  examples/cnbiloop \
		  examples/ndf \
		  examples/transpipe \
		  examples/transport
	
html: html/index.html

html/index.html:
	doxygen $(builddir)/doxygen.cfg

installdirs-local: installdirs-html

installdirs-html:
	$(INSTALL) -d $(htmldir)

install-html-local: html/index.html installdirs-html
	cp -ru html $(htmldir)

uninstall-local:
	rm -rf $(htmldir)/html

clean-local:
	rm -rf html
