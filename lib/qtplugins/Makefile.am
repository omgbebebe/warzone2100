AM_CPPFLAGS = $(WZ_CPPFLAGS) $(QT4_CFLAGS) $(GLEW_CFLAGS)
AM_CFLAGS = $(WZ_CFLAGS)
AM_CXXFLAGS = $(WZ_CXXFLAGS) $(QT4_CFLAGS)

MOCHEADER = \
	qtplugins.h

MOCEDFILES = $(MOCHEADER:%.h=%_moc.cpp)

%_moc.cpp: %.h
	$(MOC4) -o $@ $<

CLEANFILES = \
	$(MOCEDFILES)

noinst_LIBRARIES = libqtplugins.a

noinst_HEADERS = \
	$(MOCHEADER) \
	qtplugins.h

nodist_libqtplugins_a_SOURCES = \
	qtplugins_moc.cpp

libqtplugins_a_SOURCES = \
	qtplugins.cpp
