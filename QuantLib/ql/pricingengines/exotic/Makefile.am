
AM_CPPFLAGS = -I${top_builddir} -I${top_srcdir}

this_includedir=${includedir}/${subdir}
this_include_HEADERS = \
	all.hpp \
	analyticamericanmargrabeengine.hpp \
	analyticcomplexchooserengine.hpp \
	analyticcompoundoptionengine.hpp \
	analyticeuropeanmargrabeengine.hpp \
	analyticholderextensibleoptionengine.hpp \
	analyticsimplechooserengine.hpp \
	analytictwoassetcorrelationengine.hpp \
	analyticwriterextensibleoptionengine.hpp

cpp_files = \
	analyticamericanmargrabeengine.cpp \
	analyticcomplexchooserengine.cpp \
	analyticcompoundoptionengine.cpp \
	analyticeuropeanmargrabeengine.cpp \
	analyticholderextensibleoptionengine.cpp \
	analyticsimplechooserengine.cpp \
	analytictwoassetcorrelationengine.cpp \
	analyticwriterextensibleoptionengine.cpp

if UNITY_BUILD

nodist_libExoticEngines_la_SOURCES = unity.cpp

unity.cpp: Makefile.am
	echo "/* This file is automatically generated; do not edit.     */" > $@
	echo "/* Add the files to be included into Makefile.am instead. */" >> $@
	echo >> $@
	for i in $(cpp_files); do \
		echo "#include \"${subdir}/$$i\"" >> $@; \
	done

EXTRA_DIST = $(cpp_files)

else

libExoticEngines_la_SOURCES = $(cpp_files)

endif

noinst_LTLIBRARIES = libExoticEngines.la

all.hpp: Makefile.am
	echo "/* This file is automatically generated; do not edit.     */" > ${srcdir}/$@
	echo "/* Add the files to be included into Makefile.am instead. */" >> ${srcdir}/$@
	echo >> ${srcdir}/$@
	for i in $(filter-out all.hpp, $(this_include_HEADERS)); do \
		echo "#include <${subdir}/$$i>" >> ${srcdir}/$@; \
	done
	echo >> ${srcdir}/$@
	subdirs='$(SUBDIRS)'; for i in $$subdirs; do \
		echo "#include <${subdir}/$$i/all.hpp>" >> ${srcdir}/$@; \
	done

