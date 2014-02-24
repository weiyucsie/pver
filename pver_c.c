#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "config.h"

int show_key(DBI_UNIT *key) {
	int len = cast_size_t_to_int(key->DU_LEN);
	printf("%d %.*s\n", len, len, (const char *)key->DU_DATA);
	return 0;
}

int main(int argc, char *argv[]) {
	DBI *dbp;
	DBI_UNIT key, data;
	int failTryDB = 0;
	char ports_db_path[512];

	if (argc == 2 && strcmp(argv[1], "--failtest") == 0) {
	  failTryDB = 1;
	}

	get_ports_db_path(ports_db_path, sizeof(ports_db_path));

	if (dbi_init(&dbp, ports_db_path) < 0) {
	  if (!failTryDB) {
	    fprintf(stderr, "BDB Init error\n");
	    return -1;
	  } else {
	    return 0;
	  }
	}

	if (!failTryDB) {
	  bzero(&key, sizeof(DBI_UNIT));
	  bzero(&data, sizeof(DBI_UNIT));

	  dbi_iterate(dbp, show_key);
	}

	dbi_close(dbp);

	if (!failTryDB) {
	  return 0;
	} else {
	  return 1;
	}
}
