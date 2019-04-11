#include <algorithm>
#include <gtest/gtest.h>
#include <starkit_utils/threading/multi_core.h>

using namespace starkit_utils;

// Map intervals to their length.
std::vector<int> distances(const MultiCore::Intervals& its)
{
  std::vector<int> res;
  for (auto& p : its)
  {
    res.push_back(p.second - p.first);
  }
  // sort lengths to ensure reproducible results.
  std::sort(res.begin(), res.end());
  return res;
}

TEST(multiCore, BuildIntervalsAccurate)
{
  MultiCore::Intervals expected0{ { 0, 3 }, { 3, 6 }, { 6, 9 }, { 9, 12 }, { 12, 15 } };
  EXPECT_EQ(expected0, MultiCore::buildIntervals(15, 5));

  MultiCore::Intervals expected1{ { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 4 } };
  EXPECT_EQ(expected1, MultiCore::buildIntervals(4, 4));

  MultiCore::Intervals expected2{ { 0, 2 }, { 2, 4 }, { 4, 6 }, { 6, 8 } };
  EXPECT_EQ(expected2, MultiCore::buildIntervals(8, 4));
}

TEST(multiCore, BuildIntervalsGeneral)
{
  std::vector<int> expected0{ 3, 4, 4, 4 };
  EXPECT_EQ(expected0, distances(MultiCore::buildIntervals(15, 4)));

  std::vector<int> expected1{ 2, 3, 3 };
  EXPECT_EQ(expected1, distances(MultiCore::buildIntervals(8, 3)));

  std::vector<int> expected2{ 5, 5, 5, 5, 6 };
  EXPECT_EQ(expected2, distances(MultiCore::buildIntervals(26, 5)));
}

TEST(multiCore, BuildIntervalsMoreThreads)
{
  MultiCore::Intervals expected0{ { 0, 1 }, { 1, 2 } };
  EXPECT_EQ(expected0, MultiCore::buildIntervals(2, 4));
}

// XXX: Floating point exception
TEST(multiCore, DISABLED_BuildIntervalsZeros)
{
  EXPECT_ANY_THROW(MultiCore::buildIntervals(0, 0));
  EXPECT_ANY_THROW(MultiCore::buildIntervals(1, 0));
  EXPECT_ANY_THROW(MultiCore::buildIntervals(0, 1));
}

// XXX: Floating point exception
TEST(multiCore, DISABLED_BuildIntervalsNegatives)
{
  EXPECT_ANY_THROW(MultiCore::buildIntervals(-1, 3));
  EXPECT_ANY_THROW(MultiCore::buildIntervals(1, -3));
}

TEST(multiCore, runParallelTaskBasic)
{
  int res[15];
  MultiCore::Task t = [&](int begin, int end) { res[begin] = end; };
  MultiCore::runParallelTask(t, 15, 3);
  EXPECT_EQ(5, res[0]);
  EXPECT_EQ(10, res[5]);
  EXPECT_EQ(15, res[10]);
}

TEST(multiCore, runParallelStochasticTaskBasic)
{
  int res[12];
  auto task = [&](int begin, int end, std::default_random_engine*) { res[begin] = end; };
  std::vector<std::default_random_engine> engines(2);
  MultiCore::runParallelStochasticTask(task, 12, &engines);
  EXPECT_EQ(6, res[0]);
  EXPECT_EQ(12, res[6]);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
