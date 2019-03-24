#include <gtest/gtest.h>
#include <rhoban_utils/history/history.h>

using namespace rhoban_utils;

// Verify that the constructor actually works and that the object has
// sane properties.
TEST(history, instantiate)
{
  History h;
  EXPECT_EQ(0, h.size());
}

// Check a range of timestamps/values to push in the history.
TEST(history, addIncrementingTimestamps)
{
  History h;
  h.pushValue(3., 1.);
  EXPECT_EQ(1, h.size());
  h.pushValue(3.2, 3.);
  EXPECT_EQ(2, h.size());
  h.pushValue(3.5, 3.);
  EXPECT_EQ(3, h.size());
  h.pushValue(4.2, 3.);
  EXPECT_EQ(4, h.size());
}

// Violate the condition of incrementing the timestamp
TEST(history, addDecrementingTimeStamps)
{
  History h;
  h.pushValue(3., 1.);
  ASSERT_THROW(h.pushValue(2., 33.), std::logic_error);
}

// Check that putting the same timestamp is harmless.
TEST(history, addSameTimestamp)
{
  History h;
  h.pushValue(4.2, 3.);
  h.pushValue(4.2, 4.);
  EXPECT_EQ(1, h.size());
  EXPECT_DOUBLE_EQ(3., h.back().second);
}

// Check that behavior of front and back methods are consistent.
TEST(history, checkFrontBack)
{
  History h;
  h.pushValue(1., 0.);
  EXPECT_DOUBLE_EQ(1., h.front().first);
  EXPECT_DOUBLE_EQ(1., h.back().first);
  EXPECT_DOUBLE_EQ(0., h.front().second);
  EXPECT_DOUBLE_EQ(0., h.back().second);

  h.pushValue(2., 1.);
  EXPECT_DOUBLE_EQ(1., h.front().first);
  EXPECT_DOUBLE_EQ(0., h.front().second);
  EXPECT_DOUBLE_EQ(2., h.back().first);
  EXPECT_DOUBLE_EQ(1., h.back().second);

  h.pushValue(3., 2.);
  EXPECT_DOUBLE_EQ(1., h.front().first);
  EXPECT_DOUBLE_EQ(0., h.front().second);
  EXPECT_DOUBLE_EQ(3., h.back().first);
  EXPECT_DOUBLE_EQ(2., h.back().second);
}

// Check that casual history values are properly logged.
TEST(history, loggingBasic)
{
  History h;
  h.startLogging();
  h.pushValue(1., 2.);
  h.pushValue(3., 4.);
  std::ostringstream log;
  h.stopLogging(log);
  EXPECT_EQ(std::string{ "1 2\n3 4\n" }, log.str());
}

// Check that history values starting at 0 are properly logged.
// TODO: Re-enable this test when the issue is fixed
TEST(history, DISABLED_loggingFromZero)
{
  History h;
  h.startLogging();
  h.pushValue(0., 1.);
  h.pushValue(2., 3.);
  std::ostringstream log;
  h.stopLogging(log);
  // FIXME: log.str() returns "";
  EXPECT_EQ(std::string{ "0 1\n2 3\n" }, log.str());
}

// Check numbers interpolation
TEST(history, interpolationNumber)
{
  History h(8.);
  h.pushValue(1., 2.);
  h.pushValue(3., 4.);
  h.pushValue(5., 12.);
  EXPECT_DOUBLE_EQ(3., h.interpolate(2.));
  EXPECT_DOUBLE_EQ(3.5, h.interpolate(2.5));
  EXPECT_DOUBLE_EQ(6., h.interpolate(3.5));
  EXPECT_DOUBLE_EQ(8., h.interpolate(4.));
  EXPECT_DOUBLE_EQ(10., h.interpolate(4.5));
}

// Check angles interpolation
TEST(history, interpolationAngleRad)
{
  History h(8.);
  h.pushValue(1., 2.);
  h.pushValue(3., 4.);
  h.pushValue(5., 12.);
  EXPECT_DOUBLE_EQ(3., h.interpolate(2., History::AngleRad));
  EXPECT_DOUBLE_EQ(-2.6215653753294101, h.interpolate(2.5, History::AngleRad));
  EXPECT_DOUBLE_EQ(-1.9495462246455295, h.interpolate(3.5, History::AngleRad));
  EXPECT_DOUBLE_EQ(-1.4247779607693793, h.interpolate(4., History::AngleRad));
  EXPECT_DOUBLE_EQ(-0.90000969689322929, h.interpolate(4.5, History::AngleRad));
}

// Check that interpolation handles timestamp outside of the range of
// stored ones.
TEST(history, interpolationLimits)
{
  History h;
  EXPECT_NO_THROW(h.interpolate(0.5));
  h.pushValue(1., 2.);
  h.pushValue(3., 4.);
  EXPECT_NO_THROW(h.interpolate(3.5));
}

// Check that invalid valueType argument actually throws.
TEST(history, interpolationInvalidValueType)
{
  History h(3.);
  h.pushValue(1., 2.);
  h.pushValue(3., 4.);
  EXPECT_THROW(h.interpolate(2., History::ValueType(-1)), std::logic_error);
  EXPECT_THROW(h.interpolate(2., History::ValueType(2)), std::logic_error);
}

// Check loadReplay with basic scenario.
TEST(history, loadReplay)
{
  History h;
  std::istringstream is{ "1 2\n3 4\n" };
  h.loadReplay(is);
  EXPECT_DOUBLE_EQ(h.front().first, 1.);
  EXPECT_DOUBLE_EQ(h.front().second, 2.);
  EXPECT_DOUBLE_EQ(h.back().first, 3.);
  EXPECT_DOUBLE_EQ(h.back().second, 4.);
}

// Check loadReplay with timeshift argument.
TEST(history, loadReplayTimeShift)
{
  History h;
  std::istringstream is{ "1 2\n3 4\n" };
  h.loadReplay(is, false, 1.0);
  EXPECT_DOUBLE_EQ(2., h.front().first);
  EXPECT_DOUBLE_EQ(2., h.front().second);
  EXPECT_DOUBLE_EQ(4., h.back().first);
  EXPECT_DOUBLE_EQ(4., h.back().second);
}

// Check loadReplay when the input contains a comment.
TEST(history, loadReplayComment)
{
  History h;
  std::istringstream is{ "1 2\n#comment\n3 4\n" };
  // Comment detection must stop loading process.
  h.loadReplay(is);
  EXPECT_DOUBLE_EQ(1., h.front().first);
  EXPECT_DOUBLE_EQ(2., h.front().second);
  EXPECT_DOUBLE_EQ(1., h.back().first);
  EXPECT_DOUBLE_EQ(2., h.back().second);
}

// loadReplay with decreasing timestamps
TEST(history, loadReplayDecreasing)
{
  History h;
  std::istringstream is{ "3 2\n1 4\n" };
  ASSERT_THROW(h.loadReplay(is), std::runtime_error);
}

// For binary formatting ensure that what is logged can then be
// replayed.
TEST(history, binary)
{
  History h1;
  h1.startLogging();
  h1.pushValue(1., 2.);
  h1.pushValue(3., 4.);
  std::ostringstream log;
  h1.stopLogging(log, true);

  History h2;
  std::istringstream is{ log.str() };
  h2.loadReplay(is, true);
  EXPECT_DOUBLE_EQ(h2.front().first, 1.);
  EXPECT_DOUBLE_EQ(h2.front().second, 2.);
  EXPECT_DOUBLE_EQ(h2.back().first, 3.);
  EXPECT_DOUBLE_EQ(h2.back().second, 4.);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
