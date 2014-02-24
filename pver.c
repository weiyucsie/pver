#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "config.h"

static void process_entry(PVER_ENTRY_DATA *entry_data) {
  DBI_UNIT key, data;

  /**
   * Get port info by the package name
   */
  if (dbi_get(entry_data->ports_db, &key, &data, entry_data->pkg_full_name, entry_data->pkg_full_name_len) == DB_NOTFOUND) {
    int origin_len;

    if (!entry_data->pkg_origin) {
      entry_data->pkg_origin = entry_data->get_origin_callback(entry_data->pkg_full_name);
    }
    if (entry_data->pkg_origin) {
      origin_len = strlen(entry_data->pkg_origin);

      /**
       * Get port info by the package origin
       * Found -> upgraded(port origin unchanged, and port version changed)
       * Not found -> moved or removed
       */
      if (dbi_get(entry_data->ports_db, &key, &data, entry_data->pkg_origin, origin_len) == DB_NOTFOUND) {
	char moved_loc[512] = "\0";

	if (!get_moved_port(entry_data->pkg_origin, origin_len, moved_loc, sizeof(moved_loc))) {
	  /**
	   * get_moved_port
	   * 0 Found
	   */
	  printf("%-32s\t", entry_data->pkg_full_name);
	  if (moved_loc[0]) {
	    printf("Port move to %s", moved_loc);
	  } else {
	    printf("Port removed from %.*s", origin_len, entry_data->pkg_origin);
	  }
	} else {
	  /**
	   * get_moved_port
	   * -1 Not found
	   */
	  printf("\033[33m%-32s\tis not in " PORTSDIR "/INDEX\033[m", entry_data->pkg_full_name);
	}
	printf("\n");

      } else {
	char *port_ptr;
	char *port_version_ptr;
	/**
	 * port_ptr: data.data 's c-string version(port version)
	 */
	port_ptr = strndup((const char *)data.DU_DATA, data.DU_LEN);

	/**
	 * If string port_ptr contains |, port_ptr is terminated at |
	 * (If no | in port_ptr, still hold since it's a c-string version
	 * where port_ptr is port-name
	 */
	split_by_char(port_ptr, '|');

	port_version_ptr = strrchr(port_ptr, '-');
	if (entry_data->pkg_version && port_version_ptr) {
	  port_version_ptr++;
	  if (strcmp(entry_data->pkg_version, port_version_ptr) != 0) {
	    printf("%-32.*s\t%-16s\t%s\n", entry_data->pkg_name_len, entry_data->pkg_full_name, entry_data->pkg_version, port_version_ptr);
	  }
	} else {
	  if (strcmp(entry_data->pkg_full_name, port_ptr) != 0) {
	    printf("%-32s\t%s\n", entry_data->pkg_full_name, port_ptr);
	  }
	}
	free(port_ptr);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  DBI *dbp;
  char ports_db_path[512];

  get_ports_db_path(ports_db_path, sizeof(ports_db_path));

  if (dbi_init(&dbp, ports_db_path) < 0) {
    fprintf(stderr, "BDB Init error (errno = %d)\n", errno);
    return -1;
  }

#ifdef PKGNG
  pkgng_for_each(dbp, process_entry);
#else
  package_for_each(dbp, process_entry);
#endif

  dbi_close(dbp);

  return 0;
}
