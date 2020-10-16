#include <stdio.h>
#include <stdlib.h>

#include "unit_util.h"

ut_system *kUnitSystem = NULL;

void InitUnitSystem() {
  ut_set_error_message_handler(ut_ignore);
  kUnitSystem = ut_read_xml(NULL);
  ut_set_error_message_handler(ut_write_to_stderr);
}

void FreeUnitSystem() { ut_free_system(kUnitSystem); }

int BuildConverter(const char *source, const char *target,
                   ConverterContainer *container) {
  container->cv = NULL;
  container->have_unit = NULL;
  container->want_unit = NULL;
  if (source == NULL || target == NULL) {
    return converter_ok;
  }
  ut_unit *source_unit = ut_parse(kUnitSystem, source, UT_ASCII);
  ut_unit *target_unit = ut_parse(kUnitSystem, target, UT_ASCII);

  char buf1[128];
  char buf2[128];
  unsigned opts = UT_ASCII | UT_NAMES;
  ut_format(source_unit, buf1, sizeof(buf1), opts);
  ut_format(target_unit, buf2, sizeof(buf2), opts);
  container->cv = ut_get_converter(source_unit, target_unit);
  if (container->cv == NULL) {
    return converter_error;
  }
  container->have_unit = source_unit;
  container->want_unit = target_unit;
  return converter_ok;
}

void FreeConverterContainer(ConverterContainer *cc) {
  if (cc->cv != NULL)
    cv_free(cc->cv);
  if (cc->have_unit != NULL)
    ut_free(cc->have_unit);
  if (cc->want_unit != NULL)
    ut_free(cc->want_unit);
  cc->cv = NULL;
  cc->have_unit = NULL;
  cc->want_unit = NULL;
}

float ConvertValue(const cv_converter *converter, const float val) {
  if (converter == NULL) {
    return val;
  } else {
    return cv_convert_float(converter, val);
  }
}