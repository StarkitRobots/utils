#include <iostream>
#include <gtest/gtest.h>
#include <rhoban_utils/control/bang_bang.h>
#include <Eigen/Core>

using namespace rhoban_utils;

// Since max_val and max_acc agre puclic,
// we test these values to
TEST(testBangBang, instantiation)
{
  double max_vel = 2.0;
  double max_acc = 5.0;
  BangBang bb(max_vel, max_acc);

  EXPECT_EQ(max_vel, bb.max_vel);
  EXPECT_EQ(max_acc, bb.max_acc);
}

// getTime calculate the time needed for
// an object to go from point A to point B.
TEST(testBangBang, getTime)
{
  double max_vel = 2.0;
  double max_acc = 5.0;
  BangBang bb(max_vel, max_acc);

  Eigen::VectorXd src(2);
  src(0) = 0.0;
  src(1) = 0.0;
  Eigen::VectorXd dst(2);
  dst(0) = 15.0;
  dst(1) = 15.0;

  EXPECT_DOUBLE_EQ(11.006601717798214, bb.getTime(src, dst));
}

// getPosition returns position of a moving object
// from point A to point B at n second after start.
TEST(testBangBang, getPosition)
{
  double max_vel = 2.0;
  double max_acc = 5.0;
  BangBang bb(max_vel, max_acc);

  Eigen::VectorXd src(2);
  src(0) = 0.0;
  src(1) = 0.0;
  Eigen::VectorXd dst(2);
  dst(0) = 15.0;
  dst(1) = 15.0;

  Eigen::VectorXd expectedResult(2);
  expectedResult(0) = 15.0;
  expectedResult(1) = 15.0;

  EXPECT_DOUBLE_EQ(expectedResult(0), bb.getPosition(src, dst, 15.0)(0));
  EXPECT_DOUBLE_EQ(expectedResult(1), bb.getPosition(src, dst, 15.0)(1));
}

// Negative time input is NOT checked in this fuction.
// This should NOT return anything.
TEST(testBangBang, DISABLED_getPositionNegativeTime)
{
  double max_vel = 2.0;
  double max_acc = 5.0;
  BangBang bb(max_vel, max_acc);

  Eigen::VectorXd src(2);
  src(0) = 0.0;
  src(1) = 0.0;
  Eigen::VectorXd dst(2);
  dst(0) = 15.0;
  dst(1) = 15.0;

  ASSERT_ANY_THROW(bb.getPosition(src, dst, -15.0));
}

// getPosition should return src vector at time 0
TEST(testBangBang, getPositionZero)
{
  double max_vel = 2.0;
  double max_acc = 5.0;
  BangBang bb(max_vel, max_acc);

  Eigen::VectorXd src(2);
  src(0) = 0.0;
  src(1) = 0.0;
  Eigen::VectorXd dst(2);
  dst(0) = 15.0;
  dst(1) = 15.0;

  EXPECT_EQ(src, bb.getPosition(src, dst, 0.0));
}

//TODO: Better test. Complex cases.
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
