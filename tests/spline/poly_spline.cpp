#include <gtest/gtest.h>
#include <rhoban_utils/spline/poly_spline.h>

using namespace rhoban_utils;

TEST(splinePolySpline, instantiate)
{
  ASSERT_NO_THROW(PolySpline ps);
}

// Check adding a valid range of points.
TEST(splinePolySpline, addPoints)
{
  PolySpline ps;
  ps.addPoint(3., 4.5, 0.1);
  ps.addPoint(5., 4.6, 0.1);
  ps.addPoint(6.4, 12.8, 0.1);
  ps.addPoint(7.3, 9.34, 0.1);
  EXPECT_NEAR(4.5, ps.get(3.), 0.01);
  EXPECT_NEAR(4.6, ps.get(5.), 0.01);
  EXPECT_NEAR(12.8, ps.get(6.4), 0.01);
  EXPECT_NEAR(9.34, ps.get(7.3), 0.01);
}

// Check that the addPoint support negative values.
TEST(splinePolySpline, addNegativePoints)
{
  PolySpline ps;
  ps.addPoint(-3., 4.5, 0.1);
  ps.addPoint(-5., 4.6, -0.1);
  ps.addPoint(6.4, -12.8, -0.1);
  ps.addPoint(7.3, -9.34, 0.1);
  // TODO: Find a more meaningful example.
  EXPECT_NEAR(3.1718736669312553, ps.get(-3.), 0.01);
  EXPECT_NEAR(4.6, ps.get(-5.), 0.01);
  EXPECT_NEAR(-12.8, ps.get(6.4), 0.01);
  EXPECT_NEAR(-9.34, ps.get(7.3), 0.01);
}

// Check that the addPoint support zero values
TEST(splinePolySpline, addZeroPoints)
{
  PolySpline ps;
  ps.addPoint(0., 4.6, 0.3);
  ps.addPoint(6.4, 0., 2);
  ps.addPoint(7.3, 3., 0.);
  EXPECT_NEAR(4.6, ps.get(0.), 0.01);
  EXPECT_NEAR(0., ps.get(6.4), 0.01);
  EXPECT_NEAR(3., ps.get(7.3), 0.01);
}

// Check if the `get` method has holes in its domain.
TEST(splinePolySpline, getDomain)
{
  PolySpline ps;
  ps.addPoint(3., 4.5, 0.1);
  ps.addPoint(5., 4.6, 0.2);
  EXPECT_NO_THROW(ps.get(-1.));
  EXPECT_NO_THROW(ps.get(6.));
}

// Check `get` results are correctly interpolated.
TEST(splinePolySpline, getValid)
{
  PolySpline ps;
  // TODO: Find a more meaningful example.
  ps.addPoint(0., 0., 1.);
  ps.addPoint(3., 3., 0.5);
  EXPECT_NEAR(0.888, ps.get(1.), 0.01);
  EXPECT_NEAR(2.222, ps.get(2.), 0.01);
  ps.addPoint(5., 2., 0.);
  EXPECT_NEAR(2.5625, ps.get(4.), 0.01);
}

// Check that the getMod results corresponding to the repetition of
// the function described stored points in the extended domain.
TEST(splinePolySpline, getMod)
{
  PolySpline ps;
  // TODO: Find a more meaningful example.
  ps.addPoint(0., 0., 1.);
  ps.addPoint(3., 3., 0.);
  EXPECT_NEAR(0., ps.getMod(4.), 0.01);
  EXPECT_NEAR(0.92459, ps.getMod(5.999), 0.01);
  EXPECT_NEAR(0., ps.getMod(6.0001), 0.01);
}

// Check the `points` method
TEST(splinePolySpline, points)
{
  PolySpline ps;
  ps.addPoint(1., 2., 0.1);
  ps.addPoint(3., 3., 0.1);
  ps.addPoint(0., 0., 0.1);
  const PolySpline::Points pts = ps.points();

  EXPECT_DOUBLE_EQ(1., pts[0].position);
  EXPECT_DOUBLE_EQ(2., pts[0].value);
  EXPECT_DOUBLE_EQ(0.1, pts[0].delta);
  EXPECT_DOUBLE_EQ(3., pts[1].position);
  EXPECT_DOUBLE_EQ(3., pts[1].value);
  EXPECT_DOUBLE_EQ(0.1, pts[1].delta);
  EXPECT_DOUBLE_EQ(0., pts[2].position);
  EXPECT_DOUBLE_EQ(0., pts[2].value);
  EXPECT_DOUBLE_EQ(0.1, pts[2].delta);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
