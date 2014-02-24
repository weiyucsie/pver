#ifdef BSD_DB
#include <db.h>
#else
#include <db_185.h>
#endif
#include <stdio.h>
#include <strings.h>
#ifdef BSD_DB
#include <fcntl.h>
#endif
#include "config.h"

int dbi_iterate(DBI *dbp, int (*callback)(DBI_UNIT *key)) {
  int ret;
  DBI_UNIT key, data;

  /**
   * 0 for success
   * 1 for not found
   * -1 for error
   */
  while ((ret = dbp->seq(dbp, &key, &data, R_NEXT)) == 0) {
    callback(&key);
  }
  return 0;
}

int bdb_init_file(DB **dbpRef, const char *database_filename, int flag) {
  if ((*dbpRef = dbopen(database_filename, flag, 0644, DB_BTREE, NULL)) == NULL) {
    return -1;
  }
  return 0;
}

int bdb_init(DB **dbpRef, const char *database_filename) {
  return bdb_init_file(dbpRef, database_filename, O_RDONLY);
}

void bdb_close(DB *dbp) {
  if (dbp != NULL) {
    dbp->close(dbp);
  }
}

int bdb_get(DB *dbp, DBT *ptr_key, DBT *ptr_data, char *key_str, int key_len) {
  bzero(ptr_key, sizeof(DBT));
  bzero(ptr_data, sizeof(DBT));
  ptr_key->data = key_str;
  ptr_key->size = key_len;

  return dbp->get(dbp, ptr_key, ptr_data, 0);
}

int bdb_put(DB *dbp, char *key_str, int key_len, char *data_str, int data_len) {
  DBT key, data;
  bzero(&key, sizeof(DBT));
  bzero(&data, sizeof(DBT));
  key.data = key_str;
  key.size = key_len;
  data.data = data_str;
  data.size = data_len;

  return dbp->put(dbp, &key, &data, R_NOOVERWRITE);
}
