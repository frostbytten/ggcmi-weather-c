#include "gtest/gtest.h"
extern "C" {
#include "../location.h"
}
#include "location-test.h"

TEST(LocationTest, xy_string_correct) {
  const char* xy_cmp = "[7][9]";
  char xy_buf[XY_STRING_LEN];
  XY pos = XYPosition(7, 9);
  ASSERT_EQ(0, XYAsString(pos, xy_buf));
  ASSERT_STREQ(xy_buf, xy_cmp);
}

TEST(LocationTest, lonlat_string_correct) {
  char ll_buf[LONLAT_STRING_LEN];
  LonLat pos = LonLatPosition(GNV_GRID_LON, GNV_GRID_LAT);
  ASSERT_EQ(0, LonLatAsString(pos, ll_buf));
  ASSERT_STREQ(ll_buf, "(29.25,-82.75)");
}

// Because there is no 0.0,0.0 latitude and longitude reflected in the
// ISIMIP protocols, the center grid cell is actually located at
// (-0.25,0.25) (0.25W,0.25N) which is in the ocean.

TEST(LocationTest, center_ll_conversion_correct) {
  LonLat pos = LonLatPosition(C_GRID_LON, C_GRID_LAT);
  XY expected = XYPosition(359, 179);
  XY actual = LonLatToXY(pos);
  EXPECT_EQ(expected.x, actual.x);
  EXPECT_EQ(expected.y, actual.y);
}

TEST(LocationTest, center_xy_conversion_correct) {
  XY pos = XYPosition(359, 179);
  LonLat expected = LonLatPosition(-0.25, 0.25);
  LonLat actual = XYToLonLat(pos);
  EXPECT_EQ(expected.longitude, actual.longitude);
  EXPECT_EQ(expected.latitude, actual.latitude);
}

TEST(LocationTest, ll_conversion_correct) {
  LonLat pos = LonLatPosition(29.25, -82.75);
  XY expected = XYPosition(418, 345);
  XY actual = LonLatToXY(pos);
  EXPECT_EQ(expected.x, actual.x);
  EXPECT_EQ(expected.y, actual.y);
}

TEST(LocationTest, xy_lower_bounds_correct) {
  XY pos = XYPosition(0, 0);
  LonLat expected = LonLatPosition(-179.75, 89.75);
  LonLat actual = XYToLonLat(pos);
  EXPECT_EQ(expected.longitude, actual.longitude);
  EXPECT_EQ(expected.latitude, actual.latitude);
}

TEST(LocationTest, xy_upper_bounds_correct) {
  XY pos = XYPosition(719, 359);
  LonLat expected = LonLatPosition(179.75, -89.75);
  LonLat actual = XYToLonLat(pos);
  EXPECT_EQ(expected.longitude, actual.longitude);
  EXPECT_EQ(expected.latitude, actual.latitude);
}

TEST(LocationTest, xy_exceed_upper_correct) {
  XY pos = XYPosition(791, 123);
  EXPECT_EQ(pos.x, MAX_X);
  XY pos2 = XYPosition(700, 500);
  EXPECT_EQ(pos2.y, MAX_Y);
}

TEST(LocationTest, ll_exceed_lower_correct) {
  LonLat pos = LonLatPosition(-1000, -12.12);
  EXPECT_EQ(pos.longitude, -LONGITUDE_OFFSET);
  LonLat pos2 = LonLatPosition(-123.45, -700.0);
  EXPECT_EQ(pos2.latitude, -LATITUDE_OFFSET);
}

TEST(LocationTest, ll_exceed_upper_correct) {
  LonLat pos = LonLatPosition(1000, -12.12);
  EXPECT_EQ(pos.longitude, LONGITUDE_OFFSET);
  EXPECT_EQ(pos.latitude, -12.12);
  LonLat pos2 = LonLatPosition(-123.45, 700.0);
  EXPECT_EQ(pos2.longitude, -123.45);
  EXPECT_EQ(pos2.latitude, LATITUDE_OFFSET);
}

TEST(LocationTest, lonlat_lower_bounds_correct) {
  LonLat pos = LonLatPosition(-LONGITUDE_OFFSET, -LATITUDE_OFFSET);
  XY expected = XYPosition(0, MAX_Y);
  XY actual = LonLatToXY(pos);
  EXPECT_EQ(expected.x, actual.x);
  EXPECT_EQ(expected.y, actual.y);
}

TEST(LocationTest, lonlat_upper_bounds_correct) {
  LonLat pos = LonLatPosition(LONGITUDE_OFFSET, LATITUDE_OFFSET);
  XY expected = XYPosition(MAX_X, 0);
  XY actual = LonLatToXY(pos);
  EXPECT_EQ(expected.x, actual.x);
  EXPECT_EQ(expected.y, actual.y);
}

TEST(LocationTest, xy_to_globalid) {
  LonLat ll = LonLatPosition(139.25, -20.75);
  XY pos = LonLatToXY(ll);
  printf("Global ID: %zu\n", XYToGlobalId(pos));
}
