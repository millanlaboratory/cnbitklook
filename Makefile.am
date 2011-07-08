ACLOCAL_AMFLAGS=-I m4
SUBDIRS = src/ndf \
		  src/transport \
		  src/transpipe \
		  src/cnbicore \
		  src/cnbiacq \
		  src/cnbiconfig \
		  src/cnbiloop \
		  doc \
		  mex \
		  extra
		  
EXTRA_DIST = debian/changelog \
			 debian/compat \
			 debian/control\
			 debian/copyright \
			 debian/rules \
			 debian/libtobicore.install \
			 debian/libtobicore-dev.install \
			 debian/libtobicore-mex.install

REMOVE_DISTDIR = \
   { test ! -d "$(distdir)" \
   || { find "$(distdir)" -type d ! -perm -200 -exec chmod u+w {} ';' \
   && rm -fr "$(distdir)"; }; }

dist-deb: distdir
	am__cwd=`pwd` \
	&& cd $(distdir) \
	&& CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" FFLAGS="$(FFLAGS)" \
	CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LD    FLAGS)" dpkg-buildpackage \
	&& cd "$$am__cwd" 
	$(REMOVE_DISTDIR)
