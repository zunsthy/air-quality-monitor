#ifndef AIR_QUALITY_COMMON
#define AIR_QUALITY_COMMON

struct air_quality {
  int co2_ppm;
  int hcho_ppb;
  int hcho_ug;
  int tvoc_ppb;
  int tvoc_ug;
};

typedef struct air_quality aq_struct;

#endif /* AIR_QUALITY_COMMON */
