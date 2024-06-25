#include <stdio.h>
#include <unistd.h>
#include <sqlite3.h>
#include <time.h>

#include "common.h"

int store_air_quality_data(char *dbname, aq_struct* aq) {
  struct timespec tspec;
  time_t ms;
  sqlite3* db;
  sqlite3_stmt *stmt;

  if (sqlite3_open(dbname, &db)) {
    printf("open db failed: %s", sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  char* query =
    "INSERT INTO air"
    " (ts, hcho, hcho_unit, tvoc, tvoc_unit, co2, co2_unit)"
    " VALUES (?, ?, 'ppb', ?, 'ug/m3', ?, 'ppm')";
  sqlite3_prepare(db, query, -1, &stmt, NULL);

  for (;;) {
    sleep(20);

    clock_gettime(CLOCK_REALTIME, &tspec);
    ms = tspec.tv_sec * 1e3 + tspec.tv_nsec / 1e6;

    sqlite3_bind_int64(stmt, 1, ms);
    sqlite3_bind_int(stmt, 2, aq->hcho_ppb);
    sqlite3_bind_int(stmt, 3, aq->tvoc_ug);
    sqlite3_bind_int(stmt, 4, aq->co2_ppm);
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}

