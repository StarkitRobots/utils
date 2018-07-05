#include <gtest/gtest.h>
#include <rhoban_utils/spline/function.h>

using namespace rhoban_utils;

// Verify that the constructor actually works.
TEST(splineFunction, instantiate)
{
  ASSERT_NO_THROW(Function f);
}

// Check adding a valid range of points.
TEST(splineFunction, addPoints)
{
  Function f;
  f.addPoint(3., 4.5);
  f.addPoint(5., 4.6);
  f.addPoint(6.4, 12.8);
  f.addPoint(7.3, 9.34);
  EXPECT_NEAR(4.5, f.get(3.), 0.01);
  EXPECT_NEAR(4.6, f.get(5.), 0.01);
  EXPECT_NEAR(12.8, f.get(6.4), 0.01);
  EXPECT_NEAR(9.34, f.get(7.3), 0.01);
}

// Check that the addPoint support negative values
TEST(splineFunction, addNegativePoints)
{
  Function f;
  f.addPoint(-3., 4.5);
  f.addPoint(-5., 4.6);
  f.addPoint(6.4, -12.8);
  f.addPoint(7.3, -9.34);
  EXPECT_NEAR(4.5, f.get(-3.), 0.01);
  // XXX: is the imprecision normal?
  EXPECT_NEAR(4.6, f.get(-5.), 0.1);
  EXPECT_NEAR(-12.8, f.get(6.4), 0.01);
  EXPECT_NEAR(-9.34, f.get(7.3), 0.01);
}

// Check that the addPoint support zero values
TEST(splineFunction, addZeroPoints)
{
  Function f;
  f.addPoint(0., 4.6);
  f.addPoint(6.4, 0.);
  EXPECT_NEAR(4.6, f.get(0.), 0.01);
  EXPECT_NEAR(0., f.get(6.4), 0.01);
}

// Check basic getXMax call.
TEST(splineFunction, xMax)
{
  Function f;
  f.addPoint(3., 4.5);
  f.addPoint(5., 4.6);
  f.addPoint(6.4, 12.8);
  f.addPoint(7.3, 9.34);
  EXPECT_DOUBLE_EQ(f.getXMax(), 7.3);
}

// Check getXMax call when there is no point.
TEST(splineFunction, xMaxEmpty)
{
  Function f;
  EXPECT_NO_THROW(f.getXMax());
}

// Check if the `get` method has holes in its domain.
TEST(splineFunction, getDomain)
{
  Function f;
  f.addPoint(3., 4.5);
  f.addPoint(5., 4.6);
  EXPECT_NO_THROW(f.get(-1.));
  EXPECT_NO_THROW(f.get(6.));
}

// Check `get` results are correctly interpolated.
TEST(splineFunction, getValid)
{
  Function f;
  f.addPoint(0., 0.);
  f.addPoint(3., 3.);
  EXPECT_NEAR(1., f.get(1.), 0.01);
  EXPECT_NEAR(2., f.get(2.), 0.01);
  f.addPoint(5., 2.);
  EXPECT_NEAR(2.5, f.get(4.), 0.01);
}

// Check that the get method properly handle 'smooth'
TEST(splineFunction, getSmooth)
{
  Function f;
  f.addPoint(0., 0.);
  f.addPoint(3., 3.);
  // When close to a upper point the image must be higher than the
  // linear interpolation.
  EXPECT_LT(f.get(2.5), f.get(2.5, 1.0));
  // When close to a lower point the image must be lower than the
  // linear interpolation.
  EXPECT_GT(f.get(0.5), f.get(0.5, 1.0));
}

// Check that the getMod results corresponding to the repetition of
// the function described stored points in the extended domain.
TEST(splineFunction, getMod)
{
  Function f;
  f.addPoint(0., 0.);
  f.addPoint(3., 3.);
  EXPECT_NEAR(1., f.getMod(4.), 0.01);
  EXPECT_NEAR(3., f.getMod(5.999), 0.01);
  EXPECT_NEAR(0., f.getMod(6.0001), 0.01);
}

// Check the `clear` method
TEST(splineFunction, clear)
{
  Function f;
  f.addPoint(1., 2.);
  f.addPoint(3., 3.);
  f.clear();
  f.addPoint(0., 0.);
  EXPECT_DOUBLE_EQ(0., f.getXMax());
}

// Check JSON serialization/deserialization
TEST(splineFunction, DISABLED_json)
{
  // XXX: `fromFile` and `toFile` should be rewritten to use the
  // stream API instead of passing a file name which impose using the
  // file system for the JSON serialization.
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
