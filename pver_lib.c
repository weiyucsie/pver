#include <sys/utsname.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/**
 * 0.12	Change DEFAULT_OS_VERSION from 7 to __FreeBSD__
 */
#define DEFAULT_OS_VERSION __FreeBSD__

int get_os_major_version() {
  struct utsname un;
  if (uname(&un) == 0) {
    return atoi(un.release);
  } else {
    return DEFAULT_OS_VERSION;
  }
}

void get_ports_db_path(char *ports_db_path_buf, size_t buf_size) {
  snprintf(ports_db_path_buf, buf_size, INDEX_FMT, get_os_major_version());
}

int cast_size_t_to_int(size_t n) {
  return n > INT_MAX ? INT_MAX : n;
}

static int inline min(int a, int b) {
  return a > b ? b : a;
}

const char *split_by_char(char *lineBuf, char separator) {
  char *lineEndPtr = strchr(lineBuf, separator);
  if (lineEndPtr) {
    *lineEndPtr = '\0';
    return lineEndPtr + 1;
  } else {
    return NULL;
  }
}

/**
 * @return: 0 Found
 *         -1 Not found
 */
int get_moved_port(const char *origin, const int origin_len, char *new_loc, const int new_loc_len) {
  FILE *fp;
  const char moved[] = PORTSDIR "/MOVED";
#define BUF_SIZE 1024
  char buf[BUF_SIZE];
  long fpos = -1;
  long flast = 0L;

  new_loc[0] = '\0';
  fp = fopen(moved, "r");
  while (fgets(buf, sizeof(buf), fp)) {
    if (strncmp(origin, buf, origin_len) == 0 && buf[origin_len] == '|') {
      fpos = flast + origin_len + 1;
    }
    flast = ftell(fp);
  }
  if (fpos != -1) {
    char *pos_end;

    fseek(fp, fpos, SEEK_SET);
    fgets(new_loc, new_loc_len, fp);
    pos_end = strchr(new_loc, '|');
    if (pos_end) {
      *pos_end = '\0';
    }
  }
  fclose(fp);
  if (fpos != -1) {
    return 0;
  } else {
    return -1;
  }
}

