#include <pthread.h>

#include "common.h"
#include "hcho.h"
#include "tvoc.h"
#include "output.h"
#include "sql.h"

aq_struct aq = { 0, 0, 0, 0, 0 };

void *hcho_worker() {
  measure_air_quality_hcho("/dev/ttyAMA0", &aq);
}

void *tvoc_worker() {
  measure_air_quality_tvoc("/dev/ttyAMA2", &aq);
}

void *output_worker() {
  print_air_quality_data("/dev/i2c-1", &aq);
}

void *presist_worker() {
  store_air_quality_data("air_hs.sqlite", &aq);
}

int main(int argc, char *argv[]) {
  pthread_t th1, th2, th3, th4;

  pthread_create(&th1, NULL, *hcho_worker, NULL);
  pthread_create(&th2, NULL, *tvoc_worker, NULL);
  pthread_create(&th3, NULL, *output_worker, NULL);
  pthread_create(&th4, NULL, *presist_worker, NULL);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  pthread_join(th4, NULL);

  return 0;
}
