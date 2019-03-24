#include <gtest/gtest.h>
#include <rhoban_utils/control/variation_bound.h>
using namespace rhoban_utils;
using namespace std;
VariationBound bound;

using namespace rhoban_utils;
using namespace std;

TEST(controlVariationBound, updateDoubleSuccess1)
{
  VariationBound bound;
  double output = 1.55558;
  bound.update(output, 2.55558, 3.555554812, 5.0548661);

  /* maxVariation = 17.9728534859;
     delta =  2.55558 - 1.55558 = 1.0;
     output = 2.55558 + 1.0 = 3.55558  */
  EXPECT_DOUBLE_EQ(2.55558, output);
}

TEST(controlVariationBound, updateDoubleSuccess2)
{
  VariationBound bound;
  double output = 2.55558;
  bound.update(output, 12.55558, 3.555554812, 2.0548661);

  /* maxVariation = 7.3061890499
     delta = 10.0
     delta > maxVariation --> delta = maxVariation = 7.3061890499
     output =  2.55558 + 7.3061890499 = 9.8617690499 */
  EXPECT_NEAR(9.8617690499, output, 1e-9);
}

TEST(controlVariationBound, updateFloatSuccess1)
{
  VariationBound bound;
  float output = 1.0;
  bound.update(output, 2.0, 3.0, 5.0);

  /* maxVariation = 3.0*5.0 = 15.0;
     delta = 2.0 - 1.0 = 1.0;
     output = 1.0 + 1.0 = 2.0 */
  EXPECT_DOUBLE_EQ(2.0, output);
}

TEST(controlVariationBound, updateFloatSuccess2)
{
  VariationBound bound;
  double output = 2.5;
  bound.update(output, 12.5, 3.0, 2.0);

  /* maxVariation = 6.0
     delta = 10.0
     delta > maxVariation --> delta = maxVariation = 6.0
     output =  2.5 + 6.0 = 8.5 */
  EXPECT_DOUBLE_EQ(8.5, output);
}

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
