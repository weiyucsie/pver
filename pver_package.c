#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

/**
 * filter callback function (from PKGDIR)
 * since directory name in PKGDIR is the same as names of all packages
 * @return 0	if not necessary
 * 	   1	if necessary
 */
static int package_need_process(struct dirent *dp) {
  if (dp->d_type != DT_DIR) {
    return 0;
  } else if (dp->d_namlen <= 0) {
    return 0;
  } else if (dp->d_name[0] == '.') {
    /**
     * Don't handle . and ..
     */
    return 0;
  }
  return 1;
}

/**
 * Get the origin of the package
 * Returns:
 * 	Found -> Pointer to the origin
 * 	Not found ->NULL
 */
static char *package_get_origin(const char *full_package_name) {
    char *package_content_path;
    const char compare_start[] = "@comment ORIGIN:";
    const int cs_len = sizeof(compare_start)/sizeof(compare_start[0]) - 1;
    FILE *cfp;
    char *package_origin = NULL;
    char buf[1024];

    asprintf(&package_content_path, PKGDIR "%s/+CONTENTS", full_package_name);
    cfp = fopen(package_content_path, "r");
    if (cfp) {
      while (fgets(buf, sizeof(buf), cfp)) {
	if (strncmp(buf, compare_start, cs_len) == 0) {
	  package_origin = &buf[cs_len];
	  split_by_char(package_origin, '\n');
	  package_origin = strdup(package_origin);
	  break;
	}
      }
      fclose(cfp);
    }
    free(package_content_path);

    return package_origin;
}

static void package_init_data_from_dirent(PVER_ENTRY_DATA *pver_entry_data, struct dirent *dp) {
  char *pkg_ver_sp;
  pver_entry_data->pkg_full_name = strndup(dp->d_name, dp->d_namlen);
  pver_entry_data->pkg_full_name_len = dp->d_namlen;

  pkg_ver_sp = strrchr(pver_entry_data->pkg_full_name, '-');
  if (pkg_ver_sp) {
    pver_entry_data->pkg_name_len = pkg_ver_sp - pver_entry_data->pkg_full_name;
    pver_entry_data->pkg_version = pkg_ver_sp + 1;
  } else {
    pver_entry_data->pkg_name_len = pver_entry_data->pkg_full_name_len;
    pver_entry_data->pkg_version = NULL;
  }
}

static void package_free_data(PVER_ENTRY_DATA *pver_entry_data) {
  free(pver_entry_data->pkg_full_name);
  pver_entry_data->pkg_full_name = NULL;
  if (pver_entry_data->pkg_origin) {
    free(pver_entry_data->pkg_origin);
    pver_entry_data->pkg_origin = NULL;
  }
}

void package_for_each(DBI *dbp, void (*process_entry_callback)(PVER_ENTRY_DATA *)) {
  PVER_ENTRY_DATA pver_entry_data;
  pver_entry_data.ports_db = dbp;
  pver_entry_data.get_origin_callback = package_get_origin;
#if SORT_PACKAGE
  struct dirent **namelist;
  int n_package = scandir(PKGDIR, &namelist, package_need_process, alphasort);
  int i;

  for (i = 0; i < n_package; i++) {
    package_init_data_from_dirent(&pver_entry_data, namelist[i]);
    process_entry_callback(&pver_entry_data);
    package_free_data(&pver_entry_data);
    free(namelist[i]);
  }
  free(namelist);
#else
  DIR *dirp;
  struct dirent *dp;

  dirp = opendir(PKGDIR);
  while ((dp = readdir(dirp)) != NULL) {
    /**
     * We only handle directory name(pkg-name)
     * The remainding part will only handle the directories in /var/db/pkg
     */
    if (package_need_process(dp)) {
      package_init_data_from_dirent(&pver_entry_data, dp);
      process_entry_callback(&pver_entry_data);
      package_free_data(&pver_entry_data);
    }
  }

  if (dirp != NULL) {
    closedir(dirp);
  }
#endif
}

