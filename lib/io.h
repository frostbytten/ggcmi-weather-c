#ifndef WTH_NETCDF_HANDLER_H
#define WTH_NETCDF_HANDLER_H
#include <stdio.h>

#include <mpi.h>

#include "config.h"

typedef struct InqVars_ {
  int num_dims;
  int num_vars;
  int num_gattrs;
  int num_unlimited;
} InqVars;

typedef struct NetCdfInfo_ {
  int longitude_varid;
  int latitude_varid;
  int time_varid;
  int var_varid;
  size_t longitude_len;
  size_t latitude_len;
  size_t time_len;
  float fill_value;
  char *unit;
} NetCdfInfo;

int OpenAllDataFiles(Config *config, MPI_Comm mpi_comm, MPI_Info mpi_info);
int CloseAllDataFiles(Config *config, NetCdfInfo *info);
int InjectNetCdfInfo(Config *config, NetCdfInfo *info);
void DebugDataFiles(Config *config);
#endif // WTH_NETCDF_HANDLER_H
