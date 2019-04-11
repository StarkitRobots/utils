#include <gtest/gtest.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "starkit_utils/timing/sleep.h"

using namespace starkit_utils;

// test ms_sleep method
TEST(sleep, test_msSleep)
{
  clock_t t;
  t = clock();

  // t should be gretter then 0
  EXPECT_GT(t, 0);

  // sleep for 10 ms
  ms_sleep(10);
  t = clock() - t;

  // compute time "s" spent on ms_sleep
  float a = ((float)t / CLOCKS_PER_SEC);

  EXPECT_GT(a, pow(10, -5));
}

// test us_sleep method
TEST(sleep, test_usSleep)
{
  clock_t t;
  t = clock();

  // t should be gretter then 0
  EXPECT_GT(t, 0);

  // sleep for 10 us
  us_sleep(10);
  t = clock() - t;

  // compute time "s" spent on us_sleep
  float a = ((float)t / CLOCKS_PER_SEC);

  EXPECT_GT(a, pow(10, -8));
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}