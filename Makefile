PORTSDIR?=		/usr/ports
LOCALBASE?=		/usr/local
PREFIX?=		.

CC?=			clang
AR?=			ar
RM?=			rm -f
RMDIR?=			rmdir

CFLAGS+=		-Wall
CFLAGS+=		-DPORTSDIR="\"${PORTSDIR}\""
INDEX_DB_FILE?=		${PORTSDIR}/INDEX-%d$(DB_SUFFIX)
CFLAGS+=		-D'INDEX_FMT="$(INDEX_DB_FILE)"'
.ifdef SORT
CFLAGS+=		-DSORT_PACKAGE
.endif
.ifdef NDBM
CFLAGS+=		-DNDBM
DB_SUFFIX=
LIBOBJ=			pver_ndbm.o
.else
.include <Makefile.bdb>

DB_SUFFIX=		.db
BIN_DIR=		$(PREFIX)/bin
LIBOBJ=			pver_bdb.o
.endif
OBJ=			pver.o pver_c.o pver_dbgen.o
LIBOBJ+=		pver_lib.o pver_dbi.o
BIN=			$(OBJ:S/.o//)
INSTALL_FILES=		$(BIN) wpver
LIBNAME=		pver
LIBARCHIVE=		lib$(LIBNAME).a
CURRENT_OS_VER!=	uname -r|cut -d. -f 1
TMP_IDB_FILE=		$(INDEX_DB_FILE:S/%d/$(CURRENT_OS_VER)/)
TMP_IDB_DIR!=		dirname $(TMP_IDB_FILE)
INFO_DB_FILE=		pver_info_db
.if exists($(INFO_DB_FILE))
CACHE_INFO_DB!=		cat $(INFO_DB_FILE)
.else
CACHE_INFO_DB=		$(TMP_IDB_FILE)
.endif
.ifdef PKGNG
LIBOBJ+=		pver_pkgng.o
CFLAGS+=		-I$(LOCALBASE)/include -DPKGNG
LDFLAGS+=		-L$(LOCALBASE)/lib -lsqlite3
.else
LIBOBJ+=		pver_package.o
.endif

all:	$(LIBARCHIVE) $(BIN) $(CACHE_INFO_DB)
	@echo $(TMP_IDB_FILE) > $(INFO_DB_FILE)

detect_unit:
	make clean
	make $(.MAKEFLAGS)
	./pver_c --failtest

detect:
#.for dbFlag in -DNDBM -DBSD_DB ''
	make detect_unit $(.MAKEFLAGS)
.for dbFlag in -DBSD_DB -DNDBM
	make detect_unit $(.MAKEFLAGS) $(dbFlag)
.endfor

$(CACHE_INFO_DB):
makedb:
	if [ -w `dirname $(CACHE_INFO_DB)` ]; then $(.CURDIR)/pver_dbgen; fi

$(BIN):	$(.PREFIX).o
	$(CC) -o $@ $(.OODATE) $(LDFLAGS) -L. -l$(LIBNAME)

$(OBJ): $(.PREFIX).c
	$(CC) $(CFLAGS) -c -o $@ $?

$(LIBARCHIVE): $(LIBOBJ)
	$(AR) r $@ $?

$(LIBOBJ): $(.PREFIX).c
	@$(CC) $(CFLAGS) -c -o $@ $?

install: all
	if [ ! -e $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi
.for bin in $(INSTALL_FILES)
	$(INSTALL) $(bin) $(BIN_DIR)/
.endfor

clean:
	$(RM) *.o
	$(RM) $(BIN)
	$(RM) $(LIBARCHIVE)
.for bin in $(INSTALL_FILES)
	$(RM) $(BIN_DIR)/$(bin)
.endfor
	if [ -d $(BIN_DIR) ]; then $(RMDIR) $(BIN_DIR); fi
	if [ -w `dirname $(CACHE_INFO_DB)` ]; then $(RM) $(CACHE_INFO_DB); fi
	$(RM) $(INFO_DB_FILE)
