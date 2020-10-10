#ifndef WTH_CONFIG_H
#define WTH_CONFIG_H
#include <stddef.h>

typedef struct FileConfig_ {
    char* file_name;
    char* netcdf_var;
    char* dssat_var;
    char* source_unit;
    char* target_unit;
    int netcdf_id;
    int is_temp;
} FileConfig;

typedef struct Config_ {
  int start_year;
  char* station_vector;
  char* station_id;
  size_t num_mappings;
  FileConfig mappings[];
} Config;

Config *LoadConfig(const char *source);
void FreeConfig(Config *config);
#endif
