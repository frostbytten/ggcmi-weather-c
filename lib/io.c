#include <mpi.h>
#include <netcdf.h>
#include <netcdf_par.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "io.h"

static const char *kLongitudeString = "lon";
static const char *kLatitudeString = "lat";
static const char *kTimeString = "time";
static const char *kFillValueString = "missing_value";
static const char *kUnitString = "units";

int OpenAllDataFiles(Config *config, MPI_Comm mpi_comm, MPI_Info mpi_info) {
  for (size_t i = 0; i < config->num_mappings; ++i) {
    int status;
    if ((status =
             nc_open_par(config->mappings[i].file_name, NC_NOWRITE, mpi_comm,
                         mpi_info, &config->mappings[i].netcdf_id))) {
      fprintf(stderr, "error: cannot open file %s: %s\n",
              config->mappings[i].file_name, nc_strerror(status));
      return -1;
    } else {
      printf("Opened %s\n", config->mappings[i].file_name);
    }
  }
  return config->num_mappings;
}

int InjectNetCdfInfo(Config *config, NetCdfInfo *info) {
  int status;
  char varname[NC_MAX_NAME + 1];
  InqVars inq;

  for (size_t i = 0; i < config->num_mappings; ++i) {
    int dimid;
    if ((status = nc_inq(config->mappings[i].netcdf_id, &inq.num_dims,
                         &inq.num_vars, &inq.num_gattrs, &inq.num_unlimited))) {
      fprintf(stderr, "error: cannot inquire NetCDF file #%zu: %s\n", i + 1,
              nc_strerror(status));
      return 1;
    }
    // Fill the NetCdfInfo
    if ((status = nc_inq_varid(config->mappings[i].netcdf_id, kLongitudeString,
                               &info[i].longitude_varid))) {
      fprintf(stderr, "error: cannot find longitude varid for file #%zu: %s\n",
              i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_varid(config->mappings[i].netcdf_id, kLatitudeString,
                               &info[i].latitude_varid))) {
      fprintf(stderr, "error: cannot find latitude varid for file #%zu: %s\n",
              i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_varid(config->mappings[i].netcdf_id, kTimeString,
                               &info[i].time_varid))) {
      fprintf(stderr, "error: cannot find time varid for file #%zu: %s\n",
              i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_varid(config->mappings[i].netcdf_id,
                               config->mappings[i].netcdf_var,
                               &info[i].var_varid))) {
      fprintf(stderr, "error: cannot find %s varid in file #%zu: %s\n",
              config->mappings[i].netcdf_var, i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_dimid(config->mappings[i].netcdf_id, kLongitudeString,
                               &dimid))) {
      fprintf(stderr, "error: cannot find longitude dimid for file #%zu: %s\n",
              i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_dimlen(config->mappings[i].netcdf_id, dimid,
                                &info[i].longitude_len))) {
      fprintf(
          stderr,
          "error: cannot find the dimension length for %s in file #%zu: %s\n",
          kLongitudeString, i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_dimid(config->mappings[i].netcdf_id, kLatitudeString,
                               &dimid))) {
      fprintf(stderr, "error: cannot find latitude dimid for file #%zu: %s\n",
              i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_dimlen(config->mappings[i].netcdf_id, dimid,
                                &info[i].latitude_len))) {
      fprintf(
          stderr,
          "error: cannot find the dimension length for %s in file #%zu: %s\n",
          kLatitudeString, i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_dimid(config->mappings[i].netcdf_id, kTimeString,
                               &dimid))) {
      fprintf(stderr, "error: cannot find time dimid for file #%zu: %s\n",
              i + 1, nc_strerror(status));
      return 1;
    }
    if ((status = nc_inq_dimlen(config->mappings[i].netcdf_id, dimid,
                                &info[i].time_len))) {
      fprintf(
          stderr,
          "error: cannot find the dimension length for %s in file #%zu: %s\n",
          kTimeString, i + 1, nc_strerror(status));
      return 1;
    }
    if ((status =
             nc_get_att_float(config->mappings[i].netcdf_id, info[i].var_varid,
                              kFillValueString, &info[i].fill_value))) {
      fprintf(stderr,
              "error: cannot find the fill value for %s in file #%zu: %s\n",
              config->mappings[i].netcdf_var, i + 1, nc_strerror(status));
      return 1;
    }
    size_t unit_len = 0;
    if ((status = nc_inq_attlen(config->mappings[i].netcdf_id,
                                info[i].var_varid, kUnitString, &unit_len))) {
      fprintf(
          stderr,
          "error: could not handle unit extraction (len) in file #%zu: %s\n",
          i + 1, nc_strerror(status));
      return 1;
    }
    unit_len++;
    info[i].unit = (char *)malloc(unit_len * sizeof(char *));
    memset(info[i].unit, 0, unit_len);

    if ((status =
             nc_get_att_text(config->mappings[i].netcdf_id, info[i].var_varid,
                             kUnitString, info[i].unit))) {
      fprintf(
          stderr,
          "error: could not handle unit extraction (str) in file #%zu: %s\n",
          i + 1, nc_strerror(status));
      return 1;
    }
    info[i].unit[unit_len] = '\0';
  }
  return 0;
}

int CloseAllDataFiles(Config *config, NetCdfInfo *info) {
  int retval = 0;
  int status;
  for (size_t i = 0; i < config->num_mappings; ++i) {
    if (info[i].unit != NULL) {
      free(info[i].unit);
      info[i].unit = NULL;
    }
    if (config->mappings[i].netcdf_id != -1) {
      if ((status = nc_close(config->mappings[i].netcdf_id))) {
        fprintf(stderr, "error: %s [%s]", nc_strerror(status),
                config->mappings[i].file_name);
        retval = 1;
      }
      config->mappings[i].netcdf_id = -1;
      printf("Closed %s\n", config->mappings[i].file_name);
    }
  }
  return retval;
}
