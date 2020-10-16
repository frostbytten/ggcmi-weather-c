#ifndef GGCMI_WTH_GEN__UNIT_UTIL_H_
#define GGCMI_WTH_GEN__UNIT_UTIL_H_
#include <udunits2.h>

typedef struct cv_retainer {
  cv_converter *cv;
  ut_unit *have_unit;
  ut_unit *want_unit;
} ConverterContainer;

enum { converter_ok, converter_error };

void InitUnitSystem();
void FreeUnitSystem();
int BuildConverter(const char *source, const char *target,
                   ConverterContainer *container);
void FreeConverterContainer(ConverterContainer *cc);
float ConvertValue(const cv_converter *converter, const float val);
#endif // GGCMI_WTH_GEN__UNIT_UTIL_H_
