#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "pver_dbi.h"

#ifndef NDBM
#define DB_SUFFIX ".db"
#endif
#ifndef PORTSDIR
#define PORTSDIR "/usr/ports"
#endif
#ifndef INDEX_FMT
#define INDEX_FMT PORTSDIR "/INDEX-%d" DB_SUFFIX
#endif
#define PKGDIR "/var/db/pkg/"

typedef struct PVER_ENTRY_DATA {
  DBI *ports_db;
  char *pkg_full_name;
  size_t pkg_full_name_len;
  int pkg_name_len;
  char *pkg_version;
  char *pkg_origin;
  char *(*get_origin_callback)(const char *);
} PVER_ENTRY_DATA;

int get_os_major_version();
void get_ports_db_path(char *, size_t);
int cast_size_t_to_int(size_t n);
const char *split_by_char(char *lineBuf, char separator);
int get_moved_port(const char *, const int, char *, const int);

#ifdef PKGNG
#ifndef LOCAL_SQLITE_PATH
#define LOCAL_SQLITE_PATH PKGDIR "local.sqlite"
#endif
void pkgng_for_each(DBI *dbp, void (*process_entry_callback)(PVER_ENTRY_DATA *));
#else
void package_for_each(DBI *dbp, void (*process_entry_callback)(PVER_ENTRY_DATA *));
#endif
#endif
