#include <gtest/gtest.h>
#include <stdlib.h>
#include <cmath>
#include "rhoban_utils/timing/elapse_tick.h"
#include "gtest/gtest_prod.h"
using namespace rhoban_utils;

// redefine virtual tick to test elapse_tick class

class Elapse_tick : public ElapseTick
{
protected:
  virtual bool tick(double elapsed)
  {
    if (elapsed * pow(10, 5) >= 1)
      return true;
    else
      return false;
  }

private:
  TimeStamp lastTimestamp;
  FRIEND_TEST(ElapseTick, testValues1);
  FRIEND_TEST(ElapseTick, testValues2);
  FRIEND_TEST(ElapseTick, tickTest1);
  FRIEND_TEST(ElapseTick, tickTest2);
};

Elapse_tick* elapseTick;
TEST(ElapseTick, testValues1)
{
  elapseTick = new Elapse_tick();

  EXPECT_EQ(elapseTick->hasStats, 0);
  EXPECT_EQ(elapseTick->minTime, 0);
  EXPECT_EQ(elapseTick->maxTime, 0);
  EXPECT_EQ(elapseTick->avgTime, 0);
}

TEST(ElapseTick, testValues2)
{
  elapseTick = new Elapse_tick();

  EXPECT_EQ(elapseTick->lastTimestamp.getTimeMS(), 0);

  elapseTick->lastTimestamp = std::chrono::steady_clock::now();
  EXPECT_GT(elapseTick->lastTimestamp.getTimeMS(), 0);
}

TEST(ElapseTick, tickTest1)
{
  elapseTick = new Elapse_tick();
  int milisec = 50;

  elapseTick->lastTimestamp = std::chrono::steady_clock::now();

  // cal twice tick method making sleep thread beetween two call
  elapseTick->ElapseTick::tick();

  struct timespec req = { 0, 0 };
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec*)NULL);

  elapseTick->ElapseTick::tick();

  // hassStat should be true
  EXPECT_EQ(elapseTick->hasStats, 1);

  // for the second call of tick minTime should be equal to maxTime
  EXPECT_EQ(elapseTick->minTime, elapseTick->maxTime);

  // in this case avgTime = maxTime = minTime
  EXPECT_EQ(elapseTick->avgTime, elapseTick->minTime);
  EXPECT_EQ(elapseTick->avgTime, elapseTick->maxTime);
}

TEST(ElapseTick, tickTest2)
{
  elapseTick = new Elapse_tick();
  int milisec = 50;

  elapseTick->lastTimestamp = std::chrono::steady_clock::now();

  // make sleep thread beetween call of tick method
  elapseTick->ElapseTick::tick();

  struct timespec req = { 0, 0 };
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec*)NULL);

  elapseTick->ElapseTick::tick();

  req = { 0, 0 };
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec*)NULL);

  elapseTick->ElapseTick::tick();

  // compute recent avg time
  double recentAvgTime = elapseTick->avgTime * 0.99;
  elapseTick->ElapseTick::tick();

  // minTime should be less then maxTime
  EXPECT_LT(elapseTick->minTime, elapseTick->maxTime);

  double newAvgTime = recentAvgTime + elapseTick->maxTime * 0.01;

  // compute new avg time
  int avgTimeTick = round(elapseTick->avgTime * pow(10, 4));
  int newavTimeTick = round(newAvgTime * pow(10, 4));

  // avg time should be equal to avgTime*0.99 + duration*0.01

  EXPECT_EQ(avgTimeTick, newavTimeTick);

  // elapseTick->maxTime should be gretter then avgTime
  elapseTick->ElapseTick::tick();

  EXPECT_GT(elapseTick->maxTime, elapseTick->avgTime);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
