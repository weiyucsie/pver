#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
/**
 * FreeBSD ports index (not INDEX-*.db)
 */
#define INDEX_FILE_FMT PORTSDIR "/INDEX-%d"
#define SKIP_PREFIX PORTSDIR

int main(int argc, char *argv[]) {
  DBI *dbp;

  FILE *fp;
  char index_file_name[512];
  char db_file[512];
  int os_major_version = get_os_major_version();
  char *target = db_file;

  if (argc > 1) {
    target = argv[1];
  } else {
    sprintf(db_file, INDEX_FMT, os_major_version);
  }
  if (dbi_init_file(&dbp, target, DBI_WRITE) < 0) {
    fprintf(stderr, "BDB Init error\n");
    return -1;
  }

  sprintf(index_file_name, INDEX_FILE_FMT, os_major_version);
  if ((fp = fopen(index_file_name, "r"))) {
    char buf[1024];
    char *pkg_name, *port_location;
    int pkg_name_len, port_location_len;
    int loc_shift = strlen(SKIP_PREFIX);
    char *lineEnd;

    while(fgets(buf, sizeof(buf), fp)) {
      lineEnd = strchr(buf, '\n');
      pkg_name = strtok(buf, "|");
      pkg_name_len = strlen(pkg_name);
      port_location = strtok(NULL, "|") + loc_shift;
      if (port_location[0] == '/') {
	port_location++;
      }
      port_location_len = strlen(port_location);
      dbi_put(dbp, pkg_name, pkg_name_len, port_location, port_location_len);
      dbi_put(dbp, port_location, port_location_len, pkg_name, pkg_name_len);

      if (!lineEnd) {
	int c;

	while ((c = fgetc(fp)) != EOF) {
	  if (c == '\n') {
	    break;
	  }
	}
      }
    }
    fclose(fp);
  }

  dbi_close(dbp);

  return 0;
}
