#include "gtest/gtest.h"

extern "C" {
#include "calendar.h"
}
#include "calendar-test.h"

TEST(CalendarTest, create_valid_raw_date) {
  date_t nqr_date;
  int status = CreateDate(1981, 7, 7, &nqr_date);
  ASSERT_EQ(0, status);
  ASSERT_EQ(1981, nqr_date.year);
  ASSERT_EQ(7, nqr_date.month);
  ASSERT_EQ(7, nqr_date.day_of_month);
  ASSERT_EQ(0, nqr_date.is_leap);
}

TEST(CalendarTest, invalid_raw_date_month) {
  date_t bad_date;
  int status = CreateDate(1981, 13, 1, &bad_date);
  ASSERT_EQ(1, status);
}

TEST(CalendarTest, out_of_scope_year_min) {
  date_t bad_date;
  int status = CreateDate(999, 1, 2, &bad_date);
  ASSERT_EQ(1, status);
}

TEST(CalendarTest, out_of_scope_year_max) {
  date_t bad_date;
  int status = CreateDate(12345, 6, 7, &bad_date);
  ASSERT_EQ(1, status);
}

TEST(CalendarTest, calendar_parse_correct) {
  date_t new_years;
  int status = ParseDate("2020-01-01", &new_years);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2020, new_years.year);
  ASSERT_EQ(1, new_years.month);
  ASSERT_EQ(1, new_years.day_of_month);
  ASSERT_EQ(1, new_years.is_leap);
}

TEST(CalendarTest, calendar_parse_invalid) {
  date_t bad_date;
  int t = ParseDate("2020-134-12", &bad_date);
  ASSERT_EQ(1, t);
}

TEST(CalendarTest, calendar_leap_day) {
  date_t leap_day;
  int t = ParseDate("2020-02-29", &leap_day);
  ASSERT_EQ(0, t);
  ASSERT_EQ(2020, leap_day.year);
  ASSERT_EQ(2, leap_day.month);
  ASSERT_EQ(29, leap_day.day_of_month);
  ASSERT_EQ(1, leap_day.is_leap);
}

TEST(CalendarTest, calendar_no_leap_day) {
  date_t no_leap_day;
  int t = ParseDate("2019-02-29", &no_leap_day);
  ASSERT_EQ(1, t);
}

TEST(CalendarTest, calendar_no_leap_1900) {
  date_t no_leap_day;
  int t = ParseDate("1900-02-29", &no_leap_day);
  ASSERT_EQ(1, t);
}

TEST(CalendarTest, date_to_string) {
  date_t some_day;
  char some_day_str[ISODATE_STRING_LEN];
  int status = CreateDate(1991, 5, 29, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(1991, some_day.year);
  ASSERT_EQ(5, some_day.month);
  ASSERT_EQ(29, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = DateAsString(&some_day, some_day_str);
  ASSERT_EQ(0, status);
  ASSERT_STREQ("1991-05-29", some_day_str);
}

TEST(CalendarTest, add_one_in_middle_of_month) {
  date_t some_day;
  int status = CreateDate(2109, 8, 6, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(8, some_day.month);
  ASSERT_EQ(6, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(8, some_day.month);
  ASSERT_EQ(7, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
}

TEST(CalendarTest, add_on_at_the_end_of_month) {
  date_t some_day;
  int status = CreateDate(2109, 7, 31, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(7, some_day.month);
  ASSERT_EQ(31, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(8, some_day.month);
  ASSERT_EQ(1, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
}

TEST(CalendarTest, add_on_at_the_end_of_leap_month_no_leap) {
  date_t some_day;
  int status = CreateDate(2109, 2, 28, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(2, some_day.month);
  ASSERT_EQ(28, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(3, some_day.month);
  ASSERT_EQ(1, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
}

TEST(CalendarTest, add_on_at_the_end_of_leap_month_leap) {
  date_t some_day;
  int status = CreateDate(2020, 2, 28, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2020, some_day.year);
  ASSERT_EQ(2, some_day.month);
  ASSERT_EQ(28, some_day.day_of_month);
  ASSERT_EQ(1, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2020, some_day.year);
  ASSERT_EQ(2, some_day.month);
  ASSERT_EQ(29, some_day.day_of_month);
  ASSERT_EQ(1, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2020, some_day.year);
  ASSERT_EQ(3, some_day.month);
  ASSERT_EQ(1, some_day.day_of_month);
  ASSERT_EQ(1, some_day.is_leap);
}

TEST(CalendarTest, add_on_at_the_end_of_year) {
  date_t some_day;
  int status = CreateDate(2109, 12, 31, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2109, some_day.year);
  ASSERT_EQ(12, some_day.month);
  ASSERT_EQ(31, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2110, some_day.year);
  ASSERT_EQ(1, some_day.month);
  ASSERT_EQ(1, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
}

TEST(CalendarTest, add_on_at_the_end_of_year_into_leap) {
  date_t some_day;
  int status = CreateDate(2019, 12, 31, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2019, some_day.year);
  ASSERT_EQ(12, some_day.month);
  ASSERT_EQ(31, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = AddOneDay(&some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(2020, some_day.year);
  ASSERT_EQ(1, some_day.month);
  ASSERT_EQ(1, some_day.day_of_month);
  ASSERT_EQ(1, some_day.is_leap);
}

TEST(CalendarTest, check_dssat_format_jan) {
  date_t some_day;
  char dssat2_string[6];
  int status = CreateDate(1881, 1, 8, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(1881, some_day.year);
  ASSERT_EQ(1, some_day.month);
  ASSERT_EQ(8, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = DateAsDSSAT2String(&some_day, dssat2_string);
  ASSERT_EQ(0, status);
  ASSERT_STREQ("81008", dssat2_string);
}

TEST(CalendarTest, check_dssat_format_feb_pre_leap) {
  date_t some_day;
  char dssat2_string[6];
  int status = CreateDate(1880, 2, 2, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(1880, some_day.year);
  ASSERT_EQ(2, some_day.month);
  ASSERT_EQ(2, some_day.day_of_month);
  ASSERT_EQ(1, some_day.is_leap);
  status = DateAsDSSAT2String(&some_day, dssat2_string);
  ASSERT_EQ(0, status);
  ASSERT_STREQ("80033", dssat2_string);
}

TEST(CalendarTest, check_dssat_format_mar_post_leap) {
  date_t some_day;
  char dssat2_string[6];
  int status = CreateDate(1880, 3, 14, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(1880, some_day.year);
  ASSERT_EQ(3, some_day.month);
  ASSERT_EQ(14, some_day.day_of_month);
  ASSERT_EQ(1, some_day.is_leap);
  status = DateAsDSSAT2String(&some_day, dssat2_string);
  ASSERT_EQ(0, status);
  ASSERT_STREQ("80074", dssat2_string);
}

TEST(CalendarTest, check_dssat_format_mar_no_leap) {
  date_t some_day;
  char dssat2_string[6];
  int status = CreateDate(1881, 3, 14, &some_day);
  ASSERT_EQ(0, status);
  ASSERT_EQ(1881, some_day.year);
  ASSERT_EQ(3, some_day.month);
  ASSERT_EQ(14, some_day.day_of_month);
  ASSERT_EQ(0, some_day.is_leap);
  status = DateAsDSSAT2String(&some_day, dssat2_string);
  ASSERT_EQ(0, status);
  ASSERT_STREQ("81073", dssat2_string);
}
