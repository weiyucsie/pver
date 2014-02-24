#ifdef NDBM
#include <ndbm.h>
#else
#ifdef BSD_DB
#include <db.h>
#else
#include <db_185.h>
#endif
#endif
#include <fcntl.h>

#ifdef NDBM
typedef DBM DBI;
typedef datum DBI_UNIT;
#else
typedef DB DBI;
typedef DBT DBI_UNIT;
#endif

#ifdef NDBM
#define dbi_init_file ndbm_init_file
#define dbi_init ndbm_init
#define dbi_close ndbm_close
#define dbi_get ndbm_get
#define dbi_put ndbm_put
#else
#define dbi_init_file bdb_init_file
#define dbi_init bdb_init
#define dbi_close bdb_close
#define dbi_get bdb_get
#define dbi_put bdb_put
#endif
int dbi_init_file(DBI **dbpRef, const char *database_filename, int flag);
int dbi_init(DBI **dbpRef, const char *database_filename);
void dbi_close(DBI *dbp);
int dbi_get(DBI *dbp, DBI_UNIT *ptr_key, DBI_UNIT *ptr_data, char *key_str, int key_len);
int dbi_put(DBI *dbp, char *key_str, int key_len, char *data_str, int data_len);
int dbi_iterate(DBI *dbp, int (*callback)(DBI_UNIT *key));

#ifdef NDBM
#define DU_LEN dsize
#define DU_DATA dptr
#else
#define DU_LEN size
#define DU_DATA data
#endif

#ifdef NDBM
#define DBI_WRITE (O_RDWR | O_CREAT | O_TRUNC)
#else
#define DBI_WRITE (O_RDWR | O_CREAT | O_TRUNC)
#endif

#ifndef DB_NOTFOUND
#define	DB_NOTFOUND	1
#endif
