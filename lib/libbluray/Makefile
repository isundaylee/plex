#
#  libbluray build script for OSX darwin.
#
#  A quick and dirty Makefile to download/build and install libbluray
#
#  Linux: builds using the existing libbluray make system
#
#  Usage:
#    make
#    sudo make install
#
#  Darwin only:
#    make install_xbmc

# get OS type from shell
OSTYPE	= $(shell uname)

ifeq ($(OSTYPE),Darwin)
  CFLAGS=-mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.5.sdk
  LDFLAGS=-mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.5.sdk
  CPPFLAGS=-mmacosx-version-min=10.4 -isysroot /Developer/SDKs/MacOSX10.5.sdk
else
  prefix=/usr
endif

ifeq ($(OSTYPE),Darwin)
  LIBBLURAY = libbluray/src/.libs/libbluray.dylib
else
  LIBBLURAY = libbluray/src/.libs/libbluray.so
endif

all:: $(LIBBLURAY)

$(LIBBLURAY): libbluray/configure
	make -C libbluray

libbluray/configure:
	if [ ! -d libbluray ]; then git clone git://git.videolan.org/libbluray.git; fi
	cd  libbluray; ./bootstrap
ifeq ($(OSTYPE),Darwin)
	cd  libbluray; ./configure --prefix=$(prefix) --exec-prefix=$(prefix) CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)"
else
	cd  libbluray; ./configure --prefix=$(prefix) --exec-prefix=$(prefix)
endif

ifeq ($(OSTYPE),Darwin)
install_xbmc: $(LIBBLURAY)
	cp $(LIBBLURAY) ../../system/
endif

install:
	make -C libbluray install

clean:
	make -C libbluray clean

distclean::
	rm -rf libbluray
