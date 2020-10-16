#ifndef WTH_LOCATION_H_
#define WTH_LOCATION_H_
#include <stddef.h>

#define MAX_X 719
#define MAX_Y 359
#define LONGITUDE_OFFSET 179.75
#define LATITUDE_OFFSET 89.75
#define LONGITUDE_MULTIPLIER 2
#define LATITUDE_MULTIPLIER 2
#define XY_STRING_LEN 11
#define LONLAT_STRING_LEN 17

typedef struct NetCdfPosition_ {
  size_t x;
  size_t y;
  size_t shape[2];
} NetCdfPosition;

typedef struct WorldPosition_ {
  double longitude;
  double latitude;
} WorldPosition;
typedef struct NetCdfPosition_ XY;
typedef struct WorldPosition_ LonLat;

XY XYPosition(size_t x, size_t y);
LonLat LonLatPosition(double longitude, double latitude);
XY LonLatToXY(LonLat position);
LonLat XYToLonLat(XY position);
size_t XYAsString(XY position, char *dest_str);
size_t LonLatAsString(LonLat position, char *dest_str);
size_t GenerateFileName(XY position, const char *output_dir, char *dest_str);
size_t XYToGlobalId(XY position);
#endif // WTH_LOCATION_H_
