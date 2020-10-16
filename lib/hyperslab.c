#include <stdio.h>
#include <stdlib.h>

#include "hyperslab.h"

HyperslabPosition Position(size_t day, size_t x, size_t y) {
  HyperslabPosition val = {.day = day, .x = x, .y = y, .shape = {day, y, x}};
  return val;
}

HyperslabEdges Edges(size_t days, size_t x_length, size_t y_length) {
  HyperslabEdges val = {.days = days,
                        .x_length = x_length,
                        .y_length = y_length,
                        .shape = {days, y_length, x_length}};
  return val;
}

Hyperslab CreateHyperslab(HyperslabPosition corner, HyperslabEdges edges) {
  size_t flat_size = edges.days * edges.y_length * edges.x_length;
  Hyperslab val = {.corner = corner, .edges = edges, .flat_size = flat_size};
  return val;
}

size_t HyperslabValueIndex(Hyperslab hyperslab,
                           HyperslabPosition relative_position) {
  return (relative_position.day * hyperslab.edges.y_length *
          hyperslab.edges.x_length) +
         (relative_position.y * hyperslab.edges.x_length) + relative_position.x;
}

Hyperslab *AllocateHyperslabs(HyperslabPosition offset, HyperslabEdges stride,
                              size_t num_slabs) {
  if (num_slabs <= 0) {
    return NULL;
  }
  Hyperslab *slabs = (Hyperslab *)malloc(sizeof(Hyperslab) * num_slabs);
  if (slabs == NULL) {
    fprintf(stderr, "error: unable to allocate memory for %zu slabs\n",
            num_slabs);
    return NULL;
  }
  size_t chunks_x;
  size_t chunks_y;
  size_t chunks_rem;
  if (num_slabs <= 4) {
    chunks_x = 1;
    chunks_y = num_slabs;
    chunks_rem = 0;
  } else {
    chunks_x = num_slabs / 4;
    chunks_rem = num_slabs % 4;
    chunks_y = 4;
  }
  size_t internal_stride_y = stride.y_length / chunks_y;
  size_t internal_size_y_rem = stride.y_length % chunks_y;
  size_t internal_stride_x;
  size_t internal_stride_x_rem;
  size_t temp_chunks_x;
  size_t index = 0;
  for (size_t y = 0; y < chunks_y; ++y) {
    if (y < chunks_rem) {
      temp_chunks_x = chunks_x + 1;
    } else {
      temp_chunks_x = chunks_x;
    }
    if (y == chunks_y - 1) {
      internal_stride_y += internal_size_y_rem;
    }
    for (size_t x = 0; x < temp_chunks_x; ++x) {
      internal_stride_x = stride.x_length / temp_chunks_x;
      internal_stride_x_rem = stride.x_length % temp_chunks_x;
      if (x == temp_chunks_x - 1) {
        internal_stride_x += internal_stride_x_rem;
      }
      HyperslabPosition origin =
          Position(0, offset.x + ((stride.x_length / temp_chunks_x) * x),
                   offset.y + ((stride.y_length / chunks_y) * y));
      HyperslabEdges e =
          Edges(stride.days, internal_stride_x, internal_stride_y);
      printf("%zu (%zu) - %zu (%zu)\n", y, internal_stride_y, x,
             internal_stride_x);
      slabs[index * sizeof(Hyperslab)] = CreateHyperslab(origin, e);
      ++index;
    }
  }
  return slabs;
}
