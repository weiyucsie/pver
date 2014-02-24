#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "config.h"

typedef struct PKGNG_DATA {
  void (*process_entry_callback)(PVER_ENTRY_DATA *);
  PVER_ENTRY_DATA *entry_data_ptr;
} PKGNG_DATA;

static int pkgng_set_entry_data_callback(void *data, int nCol, char **fields, char **colNames) {
  PKGNG_DATA *pkgng_data = (PKGNG_DATA *)data;
  PVER_ENTRY_DATA *entry_data = pkgng_data->entry_data_ptr;
  char *pkg_name;

  entry_data->pkg_origin = fields[0];
  pkg_name = fields[1];
  entry_data->pkg_name_len = cast_size_t_to_int(strlen(pkg_name));
  entry_data->pkg_version = fields[2];

  entry_data->pkg_full_name_len = asprintf(&entry_data->pkg_full_name, "%s-%s", pkg_name, entry_data->pkg_version);

  pkgng_data->process_entry_callback(entry_data);

  free(entry_data->pkg_full_name);

  return 0;
}

static void pkgng_set_entry_data(sqlite3 *db, PKGNG_DATA *pkgng_data) {
  char *errMsg = NULL;
  const char *sql = "SELECT origin,name,version FROM packages"
#ifdef SORT_PACKAGE
  " ORDER BY name"
#endif
  ;
  
  sqlite3_exec(db, sql, pkgng_set_entry_data_callback, pkgng_data, &errMsg);
}

void pkgng_for_each(DBI *dbp, void (*process_entry_callback)(PVER_ENTRY_DATA *)) {
  PVER_ENTRY_DATA pver_entry_data;
  PKGNG_DATA pkgng_data = {process_entry_callback, &pver_entry_data};
  sqlite3 *db = NULL;

  pver_entry_data.ports_db = dbp;
  pver_entry_data.get_origin_callback = NULL;

  sqlite3_open_v2(LOCAL_SQLITE_PATH, &db, SQLITE_OPEN_READONLY, NULL);
  pkgng_set_entry_data(db, &pkgng_data);
  sqlite3_close(db);
}
