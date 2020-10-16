#include <stdio.h>

#include "location.h"

XY XYPosition(size_t x, size_t y) {
  if (x > MAX_X) {
    fprintf(stderr,
            "warning: Bounds exceeded. Redefining x as %d in call "
            "xy_position(%zu, %zu)\n",
            MAX_X, x, y);
    x = MAX_X;
  }
  if (y > MAX_Y) {
    fprintf(stderr,
            "warning: Bounds exceeded. Redefining y as %d in call "
            "xy_position(%zu, %zu)\n",
            MAX_Y, x, y);
    y = MAX_Y;
  }
  XY pos = {.x = x, .y = y, .shape = {x, y}};
  return pos;
}

LonLat LonLatPosition(double longitude, double latitude) {
  if (longitude > LONGITUDE_OFFSET) {
    fprintf(stderr,
            "warning: Bounds exceeded. Redefining longitude as %.2f in call "
            "lonlat_position(%f, %f)\n",
            LONGITUDE_OFFSET, longitude, latitude);
    longitude = LONGITUDE_OFFSET;
  } else if (longitude < -LONGITUDE_OFFSET) {
    fprintf(stderr,
            "warning: Bounds exceeded. Redefining longitude as %.2f in call "
            "lonlat_position(%f, %f)\n",
            -LONGITUDE_OFFSET, longitude, latitude);
    longitude = -LONGITUDE_OFFSET;
  }
  if (latitude > LATITUDE_OFFSET) {
    fprintf(stderr,
            "warning: Bounds exceeded. Redefining latitude as %.2f in call "
            "lonlat_position(%f, %f)\n",
            LATITUDE_OFFSET, longitude, latitude);
    latitude = LATITUDE_OFFSET;
  } else if (latitude < -LATITUDE_OFFSET) {
    fprintf(stderr,
            "warning: Bounds exceeded. Redefining latitude as %.2f in call "
            "lonlat_position(%f, %f)\n",
            -LATITUDE_OFFSET, longitude, latitude);
    latitude = -LATITUDE_OFFSET;
  }
  LonLat pos = {.longitude = longitude, .latitude = latitude};
  return pos;
}

LonLat XYToLonLat(XY position) {
  double longitude =
      ((double)position.x / LONGITUDE_MULTIPLIER) - LONGITUDE_OFFSET;
  double latitude =
      LATITUDE_OFFSET - ((double)position.y / LATITUDE_MULTIPLIER);
  return LonLatPosition(longitude, latitude);
}

XY LonLatToXY(LonLat position) {
  size_t x =
      (size_t)((position.longitude + LONGITUDE_OFFSET) * LONGITUDE_MULTIPLIER);
  size_t y =
      (size_t)((LATITUDE_OFFSET - position.latitude) * LATITUDE_MULTIPLIER);
  return XYPosition(x, y);
}

size_t XYAsString(XY position, char *dest_str) {
  size_t size =
      snprintf(dest_str, XY_STRING_LEN, "[%zu][%zu]", position.x, position.y);
  return size >= XY_STRING_LEN;
}

size_t LonLatAsString(LonLat position, char *dest_str) {
  size_t size = snprintf(dest_str, LONLAT_STRING_LEN, "(%.2f,%.2f)",
                         position.longitude, position.latitude);
  return size >= LONLAT_STRING_LEN;
}

size_t GenerateFileName(XY position, const char *output_dir, char *dest_str) {
  size_t size =
      snprintf(dest_str, 2048, "%s%zu.WTH", output_dir, XYToGlobalId(position));
  return size >= 2048;
}

size_t XYToGlobalId(XY position) { return position.x + (position.y * 720) + 1; }