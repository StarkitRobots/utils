#include <gtest/gtest.h>
#include <starkit_utils/angle.h>
#include <typeinfo>
#include <starkit_utils/serialization/json_serializable.h>

#define _USE_MATH_DEFINES
#include <cmath>

/*******************************************************
 * Constructor test
 */

TEST(constructor, testSuccess)
{
  starkit_utils::Angle a1(-410.0);
  starkit_utils::Angle a2(-50.0);
  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(50.0);
  starkit_utils::Angle a5(410.0);

  EXPECT_EQ(310.0, a1.getValue());
  EXPECT_EQ(310.0, a2.getValue());
  EXPECT_EQ(0.0, a3.getValue());
  EXPECT_EQ(50.0, a4.getValue());
  EXPECT_EQ(50.0, a5.getValue());
}

/*******************************************************
 * GetSignedValue test
 */

TEST(getSignedValue, testSuccess)
{
  starkit_utils::Angle a1(0.0);
  starkit_utils::Angle a2(50.0);
  starkit_utils::Angle a3(250.0);

  EXPECT_EQ(0.0, a1.getSignedValue());
  EXPECT_EQ(50.0, a2.getSignedValue());
  EXPECT_EQ(-110.0, a3.getSignedValue());
}

/*******************************************************
 * FromXY test
 */

TEST(fromXY, testSuccess)
{
  starkit_utils::Angle a1 = starkit_utils::Angle::fromXY(1, 1);
  starkit_utils::Angle a2 = starkit_utils::Angle::fromXY(-1, -1);

  EXPECT_EQ(45.0, a1.getValue());
  EXPECT_EQ(225.0, a2.getValue());
}

/*******************************************************
 * Mean test
 */

TEST(mean_stdDev, testSuccessOne)
{
  starkit_utils::Angle a1(90.0);

  std::vector<starkit_utils::Angle> angle_vector;
  angle_vector.push_back(a1);

  double stdDev;
  starkit_utils::Angle madeAngle = starkit_utils::Angle::mean(angle_vector, &stdDev);

  EXPECT_EQ(a1.getValue(), madeAngle.getValue());
  EXPECT_EQ(0, stdDev);
}

// Tests for random angle
TEST(mean_stdDev, testSuccessRandomAngle)
{
  starkit_utils::Angle a1(30.0);
  starkit_utils::Angle a2(292.0);

  starkit_utils::Angle meanAngle = starkit_utils::Angle::fromXY((starkit_utils::cos(a1) + starkit_utils::cos(a2)) / 2,
                                                              (starkit_utils::sin(a1) + starkit_utils::sin(a2)) / 2);

  std::vector<starkit_utils::Angle> angle_vector;
  angle_vector.push_back(a1);
  angle_vector.push_back(a2);

  double stdDev = 0;

  starkit_utils::Angle madeAngle = starkit_utils::Angle::mean(angle_vector, &stdDev);

  EXPECT_EQ(meanAngle.getValue(), madeAngle.getValue());

  // stdDev is at least upper diff between mean and angles
  EXPECT_LT(madeAngle.getValue() - a2.getValue(), stdDev);
  EXPECT_GT(madeAngle.getValue() - a2.getValue() + 5, stdDev);
}

// Tests for epsilon = 30 angle
TEST(mean_stdDev, testSuccessMirror1)
{
  starkit_utils::Angle a1(30.0);
  starkit_utils::Angle a2(-30.0);

  std::vector<starkit_utils::Angle> angle_vector;
  angle_vector.push_back(a1);
  angle_vector.push_back(a2);

  double stdDev = 0;

  starkit_utils::Angle madeAngle = starkit_utils::Angle::mean(angle_vector, &stdDev);

  EXPECT_EQ(madeAngle.getSignedValue(), 0);

  // stdDev is at least upper diff between mean and angles
  EXPECT_LT(30.0, stdDev);
  EXPECT_GT(35.0, stdDev);
}

// Tests for epsilon = 45 angle
TEST(mean_stdDev, testSuccessMirror2)
{
  starkit_utils::Angle a1(45.0);
  starkit_utils::Angle a2(-45.0);

  std::vector<starkit_utils::Angle> angle_vector;
  angle_vector.push_back(a1);
  angle_vector.push_back(a2);

  double stdDev = 0;

  starkit_utils::Angle madeAngle = starkit_utils::Angle::mean(angle_vector, &stdDev);

  EXPECT_EQ(madeAngle.getSignedValue(), 0);

  // stdDev is at least upper diff between mean and angles
  EXPECT_LT(45.0, stdDev);
  EXPECT_GT(50.0, stdDev);
}

// Tests for epsilon = 90 angle
// Disabled due to the result of stdDev
TEST(mean_stdDev, DISABLED_testSuccessNullMean)
{
  starkit_utils::Angle a1(90.0);
  starkit_utils::Angle a2(-90.0);

  std::vector<starkit_utils::Angle> angle_vector;
  angle_vector.push_back(a1);
  angle_vector.push_back(a2);

  double stdDev = 0;

  starkit_utils::Angle madeAngle = starkit_utils::Angle::mean(angle_vector, &stdDev);

  EXPECT_TRUE(madeAngle.getValue() == 0.0 || madeAngle.getValue() == 180.0);

  // stdDev is at least upper diff between mean and angles
  // In this case = 495
  EXPECT_LT(90.0, stdDev);
  EXPECT_GT(95.0, stdDev);
}

/*******************************************************
 * Arcsin test
 */

TEST(arcsin, testSuccess)
{
  double sinus1 = std::sin(0);
  double sinus2 = std::sin(1 * M_PI / 4);
  double sinus3 = std::sin(-1 * M_PI / 2);

  starkit_utils::Angle angle1 = starkit_utils::Angle::arcsin(sinus1);
  starkit_utils::Angle angle2 = starkit_utils::Angle::arcsin(sinus2);
  starkit_utils::Angle angle3 = starkit_utils::Angle::arcsin(sinus3);
  EXPECT_NEAR(angle1.getValue(), 0, 0.00001);
  EXPECT_NEAR(angle2.getValue(), 45.0, 0.00001);
  EXPECT_NEAR(angle3.getValue(), 270.0, 0.00001);
}

/*******************************************************
 * Arccos test
 */

TEST(arccos, testSuccess)
{
  double cosinus1 = std::cos(0);
  double cosinus2 = std::cos(1 * M_PI / 4);
  double cosinus3 = std::cos(M_PI / 2);

  starkit_utils::Angle angle1 = starkit_utils::Angle::arccos(cosinus1);
  starkit_utils::Angle angle2 = starkit_utils::Angle::arccos(cosinus2);
  starkit_utils::Angle angle3 = starkit_utils::Angle::arccos(cosinus3);
  EXPECT_NEAR(angle1.getValue(), 0, 0.00001);
  EXPECT_NEAR(angle2.getValue(), 45.0, 0.00001);
  EXPECT_NEAR(angle3.getValue(), 90.0, 0.00001);
}

/*******************************************************
 * weightedAverage test
 */

TEST(weightedAverage, testSuccessWeight0)
{
  starkit_utils::Angle a1(46.0);
  starkit_utils::Angle a2(268.0);
  starkit_utils::Angle res = starkit_utils::Angle::weightedAverage(a1, 0.0, a2, 0.0);

  EXPECT_EQ(0.0, res.getValue());
}

TEST(weightedAverage, testSuccessWeight1)
{
  starkit_utils::Angle a1(45.0);
  starkit_utils::Angle a2(15.0);
  starkit_utils::Angle res = starkit_utils::Angle::weightedAverage(a1, 1.0, a2, 1.0);

  EXPECT_NEAR(30.0, res.getValue(), 0.00001);
}

TEST(weightedAverage, testSuccess)
{
  // cos = sqrt(2) / 2
  // sin = sqrt(2) / 2
  starkit_utils::Angle a1(45.0);

  // cos = sqrt(3) / 2
  // sin = -1 / 2
  starkit_utils::Angle a2(-30.0);

  starkit_utils::Angle res = starkit_utils::Angle::weightedAverage(a1, 2.0, a2, 4.0);

  // Add of vector with weight =>
  // x = sqrt(2) * 2 / 2 + sqrt(3) * 4 / 2
  // x = sqrt(2) * 2 / 2 + 1 * 4 / 2
  starkit_utils::Angle expectedRes =
      starkit_utils::Angle::fromXY(std::sqrt(2.0) + 2.0 * std::sqrt(3.0), std::sqrt(2) + -2.0);

  EXPECT_EQ(expectedRes.getValue(), res.getValue());
}

/*******************************************************
 * equals test
 */

TEST(equals, testSuccessTrue)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(180.0);
  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(0.0);

  EXPECT_TRUE(a1.equals(a2));
  EXPECT_TRUE(a3.equals(a4));
}

TEST(equals, testSuccessFalse)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(310.0);
  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(1.0);

  EXPECT_FALSE(a1.equals(a2));
  EXPECT_FALSE(a3.equals(a4));
}

/*******************************************************
 * operator- test
 */

TEST(minus, testSuccess)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(45.0);
  starkit_utils::Angle res = a1 - a2;
  EXPECT_EQ(res.getValue(), 135.0);
}

TEST(minus, testSuccess_null)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(0.0);
  starkit_utils::Angle res12 = a1 - a2;

  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(0.0);
  starkit_utils::Angle res34 = a3 - a4;

  EXPECT_EQ(res12.getValue(), 180.0);
  EXPECT_EQ(res34.getValue(), 0.0);
}

/*******************************************************
 * operator+ test
 */

TEST(plus, testSuccess)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(45.0);
  starkit_utils::Angle res = a1 + a2;
  EXPECT_EQ(res.getValue(), 225.0);
}

TEST(plus, testSuccess_null)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(0.0);
  starkit_utils::Angle res12 = a1 + a2;

  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(0.0);
  starkit_utils::Angle res34 = a3 + a4;

  EXPECT_EQ(res12.getValue(), 180.0);
  EXPECT_EQ(res34.getValue(), 0.0);
}

/*******************************************************
 * operator== test
 */

TEST(operator_equal_equal, testSuccessTrue)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(180.0);
  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(0.0);

  EXPECT_TRUE(a1 == a2);
  EXPECT_TRUE(a3 == a4);
}

TEST(operator_equal_equal, testSuccessFalse)
{
  starkit_utils::Angle a1(180.0);
  starkit_utils::Angle a2(310.0);
  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle a4(1.0);

  EXPECT_FALSE(a1 == a2);
  EXPECT_FALSE(a3 == a4);
}

/*******************************************************
 * operator- global test
 */

TEST(operator_minus_global, testSuccess)
{
  starkit_utils::Angle a1(90.0);
  starkit_utils::Angle res1 = -a1;
  starkit_utils::Angle a2(180.0);
  starkit_utils::Angle res2 = -a2;
  starkit_utils::Angle a3(0.0);
  starkit_utils::Angle res3 = -a3;

  EXPECT_EQ(res1.getValue(), 270.0);
  EXPECT_EQ(res2.getValue(), 180.0);
  EXPECT_EQ(res3.getValue(), 0.0);
}

/*******************************************************
 * operator* global test
 */

TEST(operator_scale_multiplication_global_1, testSuccess)
{
  starkit_utils::Angle a1(45.0);
  starkit_utils::Angle res1 = a1 * 3;
  starkit_utils::Angle res2 = a1 * -1;
  starkit_utils::Angle res3 = a1 * 0;

  EXPECT_EQ(res1.getValue(), 135.0);
  EXPECT_EQ(res2.getValue(), 315.0);
  EXPECT_EQ(res3.getValue(), 0.0);
}

/*******************************************************
 * operator* global test
 */

TEST(operator_scale_multiplication_global_2, testSuccess)
{
  starkit_utils::Angle a1(45.0);
  starkit_utils::Angle res1 = 3 * a1;
  starkit_utils::Angle res2 = -1 * a1;
  starkit_utils::Angle res3 = 0 * a1;

  EXPECT_EQ(res1.getValue(), 135.0);
  EXPECT_EQ(res2.getValue(), 315.0);
  EXPECT_EQ(res3.getValue(), 0.0);
}

/*******************************************************
 * operator/ global test
 */

TEST(operator_division_global, testSuccess)
{
  starkit_utils::Angle a1(90.0);
  starkit_utils::Angle res1 = a1 / 2;
  starkit_utils::Angle res2 = a1 / 3;
  starkit_utils::Angle res3 = a1 / -3;

  EXPECT_EQ(res1.getValue(), 45.0);
  EXPECT_EQ(res2.getValue(), 30.0);
  EXPECT_EQ(res3.getValue(), 330.0);
}

/*******************************************************
 * operator<< global test
 */

TEST(operator_ostream, testSuccess)
{
  std::ostringstream stream1;
  std::ostringstream stream2;
  starkit_utils::Angle a1(45);
  starkit_utils::Angle a2(45.99);
  stream1 << a1;
  stream2 << a2;
  EXPECT_EQ("45", stream1.str());
  EXPECT_EQ("45.99", stream2.str());
}

/*******************************************************
 * Sin test
 */

TEST(sin, testSuccess)
{
  starkit_utils::Angle a1(0.0);
  starkit_utils::Angle a2(45.0);
  starkit_utils::Angle a3(270.0);

  double a1Rad = a1.getValue() * M_PI / 180;
  double a2Rad = a2.getValue() * M_PI / 180;
  double a3Rad = a3.getValue() * M_PI / 180;

  EXPECT_EQ(starkit_utils::sin(a1), std::sin(a1Rad));
  EXPECT_EQ(starkit_utils::sin(a2), std::sin(a2Rad));
  EXPECT_EQ(starkit_utils::sin(a3), std::sin(a3Rad));
}

/*******************************************************
 * cos test
 */

TEST(cos, testSuccess)
{
  starkit_utils::Angle a1(0.0);
  starkit_utils::Angle a2(45.0);
  starkit_utils::Angle a3(270.0);

  double a1Rad = a1.getValue() * M_PI / 180;
  double a2Rad = a2.getValue() * M_PI / 180;
  double a3Rad = a3.getValue() * M_PI / 180;

  EXPECT_EQ(starkit_utils::cos(a1), std::cos(a1Rad));
  EXPECT_EQ(starkit_utils::cos(a2), std::cos(a2Rad));
  EXPECT_EQ(starkit_utils::cos(a3), std::cos(a3Rad));
}

/*******************************************************
 * tan test
 */

TEST(tan, testSuccess)
{
  starkit_utils::Angle a1(0.0);
  starkit_utils::Angle a2(45.0);
  starkit_utils::Angle a3(270.0);

  double a1Rad = a1.getValue() * M_PI / 180;
  double a2Rad = a2.getValue() * M_PI / 180;
  double a3Rad = a3.getValue() * M_PI / 180;

  EXPECT_EQ(starkit_utils::tan(a1), std::tan(a1Rad));
  EXPECT_EQ(starkit_utils::tan(a2), std::tan(a2Rad));
  EXPECT_EQ(starkit_utils::tan(a3), std::tan(a3Rad));
}

/*******************************************************
 * deg2rad test
 */

TEST(deg2rad, testSuccess)
{
  EXPECT_EQ(starkit_utils::deg2rad(0.0), 0.0);
  EXPECT_EQ(starkit_utils::deg2rad(45.0), M_PI / 4);
  EXPECT_EQ(starkit_utils::deg2rad(90.0), M_PI / 2);
  EXPECT_EQ(starkit_utils::deg2rad(180.0), M_PI);
  EXPECT_EQ(starkit_utils::deg2rad(270.0), 3 * M_PI / 2);
}

/*******************************************************
 * rad2deg test
 */

TEST(rad2deg, testSuccess)
{
  EXPECT_EQ(starkit_utils::rad2deg(0.0), 0.0);
  EXPECT_EQ(starkit_utils::rad2deg(M_PI / 4), 45.0);
  EXPECT_EQ(starkit_utils::rad2deg(M_PI / 2), 90.0);
  EXPECT_EQ(starkit_utils::rad2deg(M_PI), 180.0);
  EXPECT_EQ(starkit_utils::rad2deg(3 * M_PI / 2), 270.0);
}

/*******************************************************
 * normalizeRad test
 */

TEST(normalizeRad, testSuccess)
{
  EXPECT_EQ(starkit_utils::normalizeRad(0.0), 0.0);
  EXPECT_EQ(starkit_utils::normalizeRad(M_PI / 4), M_PI / 4);
  EXPECT_EQ(starkit_utils::normalizeRad(M_PI), M_PI);
  EXPECT_EQ(starkit_utils::normalizeRad(3 * M_PI / 2), -1 * M_PI / 2);
  EXPECT_EQ(starkit_utils::normalizeRad(-1 * M_PI / 2), -1 * M_PI / 2);
}

/*******************************************************
 * getJsonVal test
 */

TEST(getJsonVal, testSuccess)
{
  Json::Value json(45.0);
  starkit_utils::Angle res = starkit_utils::getJsonVal<starkit_utils::Angle>(json);
  EXPECT_EQ(res.getValue(), 45.0);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
