#include <gtest/gtest.h>
#include <rhoban_utils/control/control.h>

using namespace rhoban_utils;

// update change the value of Control depending
// on the unput value.
TEST(testControl, testControlUpdate)
{
  Control c;

  c.min = -2;
  c.max = 2;

  float testValue1 = -100.0;

  c.update(testValue1);

  ASSERT_EQ(c.output, c.min);

  float testValue2 = 35.0;

  c.update(testValue2);

  ASSERT_EQ(c.output, c.max);
}

// TODO: Better test. Complex cases.
int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
