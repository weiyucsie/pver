#include <ndbm.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include "config.h"

int dbi_iterate(DBI *dbp, int (*callback)(DBI_UNIT *key)) {
  DBI_UNIT key;
  key = dbm_firstkey(dbp);
  while (key.dptr) {
    callback(&key);
    key = dbm_nextkey(dbp);
  }
  return 0;
}

int ndbm_init_file(DBM **dbpRef, const char *database_filename, int flag) {
  *dbpRef = dbm_open(database_filename, flag, 0600);
  if ((*dbpRef) == NULL) {
    return -1;
  } else {
    return 0;
  }
}

int ndbm_init(DBM **dbpRef, const char *database_filename) {
  return ndbm_init_file(dbpRef, database_filename, O_RDONLY);
}

void ndbm_close(DBM *dbp) {
  if (dbp != NULL) {
    dbm_close(dbp);
  }
}

int ndbm_get(DBM *dbp, datum *ptr_key, datum *ptr_data, char *key_str, int key_len) {
  bzero(ptr_key, sizeof(DBT));
  bzero(ptr_data, sizeof(DBT));
  ptr_key->dptr = key_str;
  ptr_key->dsize = key_len;
  *ptr_data = dbm_fetch(dbp, *ptr_key);
  return ptr_data->dptr == NULL ? DB_NOTFOUND : !DB_NOTFOUND;
}

int ndbm_put(DBM *dbp, char *key_str, int key_len, char *data_str, int data_len) {
  datum key, data;
  key.dptr = key_str;
  key.dsize = key_len;
  data.dptr = data_str;
  data.dsize = data_len;
  dbm_store(dbp, key, data, DBM_INSERT);
  return 0;
}
