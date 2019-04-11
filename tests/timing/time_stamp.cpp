#include <gtest/gtest.h>
#include <stdlib.h>
#include "starkit_utils/timing/time_stamp.h"
#include <sys/timeb.h>
#include <time.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
using namespace starkit_utils;
using namespace std::chrono;

// test getTimeMS
TEST(time_stamp, testTime_stamp1)
{
  std::chrono::steady_clock::time_point timeValP1 = std::chrono::steady_clock::now();
  TimeStamp* time_stamp = new TimeStamp(std::chrono::steady_clock::now());
  TimeStamp* time_stamp1 = new TimeStamp();

  // time_stamp1 should have time =0
  EXPECT_EQ(time_stamp1->getTimeMS(), 0);

  // time_stamp should have time > 0
  EXPECT_GT(time_stamp->getTimeMS(), 0);

  std::chrono::steady_clock::time_point timeValP2 = std::chrono::steady_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(timeValP2 - timeValP1);

  // time_stamp should be  > 0 time_span
  EXPECT_GT(time_stamp->getTimeMS(), time_span.count());
}

// test diffSec between two time stamp on seconds
TEST(time_stamp, testDifSec)
{
  const TimeStamp* time_stamp1 = new TimeStamp(std::chrono::steady_clock::now());
  const TimeStamp* time_stamp2 = new TimeStamp(std::chrono::steady_clock::now());

  // diffS should be > 0
  double diffS = diffSec(*time_stamp1, *time_stamp2);

  EXPECT_GT(diffS, 0);
}
// test diffSec between two time stamp on milli seconds
TEST(time_stamp, testDifMSec)
{
  const TimeStamp* time_stamp1 = new TimeStamp(std::chrono::steady_clock::now());
  const TimeStamp* time_stamp2 = new TimeStamp(std::chrono::steady_clock::now());

  double diffMsec = diffMs(*time_stamp1, *time_stamp2);

  // diffMsec should be > 0
  EXPECT_GT(diffMsec, 0);
}

// test diffSec and diffMs between two time stamp
TEST(time_stamp, testDifSecMSec)
{
  const TimeStamp* time_stamp1 = new TimeStamp(std::chrono::steady_clock::now());
  const TimeStamp* time_stamp2 = new TimeStamp(std::chrono::steady_clock::now());

  double sec = diffSec(*time_stamp1, *time_stamp2);
  double msec = diffMs(*time_stamp1, *time_stamp2);

  // msec should be equal to sec*1000
  EXPECT_EQ(msec, sec * 1000);
}
// test operator witch should return true if diffMs (time_stamp1 ,time_stamp2) > 0
// return false if diffMs (time_stamp1 ,time_stamp2) < 0
TEST(time_stamp, operator_m)
{
  const TimeStamp* time_stamp1 = new TimeStamp(std::chrono::steady_clock::now());
  const TimeStamp* time_stamp2 = new TimeStamp(std::chrono::steady_clock::now());

  bool valbool1 = operator<(*time_stamp1, *time_stamp2);
  bool valbool2 = operator>(*time_stamp1, *time_stamp2);

  // operator_> should be true
  EXPECT_TRUE(valbool1);
  // operator_< should be false
  EXPECT_FALSE(valbool2);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
