#include <gtest/gtest.h>
#include <starkit_utils/control/pid.h>

using namespace std;

TEST(controlPid, computeNegatifTotal)
{
  starkit_utils::PID* pid1 = new starkit_utils::PID(1.0, 2.0, 3.0);
  float result1 = pid1->compute(-50.0, 2.0, 3.0);  // I-val = 0.0

  /* current_setpoint=-50.0;
     current_pos=2.0;
     current_error=-50.0-2.0.0=-52.0;
     P_Val =-52.0
     I_Val = 2.0*-52.0*3.0 = -312.0;
     I_Val = -100.0
     D_val = 3.0*(-52.0-0.0)/3.0 =-52.0

     prev-error = -52.0
     total = -52.0-100.0-52.0 + 9.0 =-204.0
     on retourne total = -204.0 */

  delete pid1;
  EXPECT_DOUBLE_EQ(-204.0, result1);
}

TEST(controlPid, computepositifTotal1)
{
  starkit_utils::PID* pid2 = new starkit_utils::PID(1.0, 2.0, 3.0);
  float result2 = pid2->compute(32.0, 2.0, 3.0);

  /* I-Val = 0.0
     Current_SEtPosition = 32.0
     current_pos = 2.0
     current_error = 30.0
     P_Val = 1.0*30.0 = 30.0
     I_Val = 2.0*30.0*3.0 = 180
     I_Val = 100
     D_Val = 3.0*(30.0-0.0) / 3.0 = 30.0
     prev_error = 30.0
     total = 30.0+30.0+100.0 = 160 > 100
     on retourne total */

  delete pid2;
  EXPECT_DOUBLE_EQ(160.0, result2);
}

TEST(controlPid, computePositifTotal2)
{
  starkit_utils::PID* pid3 = new starkit_utils::PID(1.0, 2.0, 3.0);
  float result3 = pid3->compute(-11.0, -2.0, -3.0);

  /* I-Val = 0.0
     Current_SetPosition = -11.0
     current_pos = -2.0
     current_error = -9.0
     P_Val = 1.0*30.0 = -9.0
     I_Val = 2.0*-9.0*-3.0 = 54.0
     D_Val = 3.0*(-9.0-0.0) / 3.0 = -9.0
     prev_error = -9.0
     total = 54.0-9.0-0.0 = 45.0 > 0
     on retourne total */

  delete pid3;
  EXPECT_DOUBLE_EQ(45.0, result3);
}

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
