#include "gtest/gtest.h"

extern "C" {
#include "hyperslab.h"
}

TEST(HyperslabTest, check_corner_shape) {
  HyperslabPosition t2 = Position(1, 2, 3);
  size_t t_len2 = sizeof(t2.shape) / sizeof(t2.shape[0]);
  EXPECT_EQ(t2.shape[0], t2.day);
  EXPECT_EQ(t2.shape[1], t2.y);
  EXPECT_EQ(t2.shape[2], t2.x);
  EXPECT_EQ(t_len2, 3);
}

TEST(HyperslabTest, check_edges_shape) {
  HyperslabEdges t3 = Edges(10, 20, 30);
  size_t t_len3 = sizeof(t3.shape) / sizeof(t3.shape[0]);
  EXPECT_EQ(t3.shape[0], t3.days);
  EXPECT_EQ(t3.shape[1], t3.y_length);
  EXPECT_EQ(t3.shape[2], t3.x_length);
  EXPECT_EQ(t_len3, 3);
}

TEST(HyperslabTest, check_hyperslab_shape) {
  Hyperslab hs1 = CreateHyperslab(Position(0, 1, 2), Edges(10, 25, 30));
  EXPECT_EQ(hs1.corner.shape[0], 0);
  EXPECT_EQ(hs1.corner.shape[1], hs1.corner.y);
}

TEST(HyperslabTest, check_hyperslab_value1_upper_bound_no_wrap) {
  Hyperslab hs = CreateHyperslab(Position(0, 0, 0), Edges(1461, MAX_X, MAX_Y));
  EXPECT_EQ(377373621, HyperslabValueIndex(hs, Position(1461, MAX_X, MAX_Y)));
  EXPECT_EQ(28341944640,
            HyperslabValueIndex(
                hs, Position(109800, MAX_X, MAX_Y))); // Approx 3000 years
}

TEST(HyperslabTest, check_hyperslab_value1_in_bounds) {
  Hyperslab hs = CreateHyperslab(Position(0, 0, 0), Edges(1461, 45, 25));
  EXPECT_EQ(1461 * 45 * 25 - 1,
            HyperslabValueIndex(hs, Position(1460, 44, 24)));
}

TEST(HyperslabTest, check_allocation_math) {
  Hyperslab *hs = NULL;
  HyperslabPosition pos = Position(0, 0, 0);
  HyperslabEdges stride = Edges(1461, 720, 360);
  hs = AllocateHyperslabs(pos, stride, 4, 1);
  free(hs);
}
