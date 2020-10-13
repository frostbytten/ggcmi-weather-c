#ifndef WTH_HYPERSLAB_H_
#define WTH_HYPERSLAB_H_
#include "location.h"
#include <netcdf.h>
#include <stddef.h>

typedef struct HyperslabPosition_ {
  size_t day;
  size_t x;
  size_t y;
  size_t shape[3];
} HyperslabPosition;

typedef struct HyperslabEdges_ {
  size_t days;
  size_t x_length;
  size_t y_length;
  size_t shape[3];
} HyperslabEdges;

typedef struct Hyperslab_ {
  HyperslabPosition corner;
  HyperslabEdges edges;
  size_t flat_size;
} Hyperslab;

typedef struct HyperslabStride_ {
  size_t x_length;
  size_t y_length;
} HyperslabStride;

HyperslabPosition Position(size_t day, size_t x, size_t y);
HyperslabEdges Edges(size_t days, size_t x_length, size_t y_length);
Hyperslab CreateHyperslab(HyperslabPosition corner, HyperslabEdges edges);
size_t HyperslabValueIndex(Hyperslab hyperslab, HyperslabPosition position);
Hyperslab *AllocateHyperslabs(HyperslabPosition offset, HyperslabEdges stride,
                              size_t num_slabs);
#endif // WTH_HYPERSLAB_H
