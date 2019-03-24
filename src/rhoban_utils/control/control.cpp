#include "rhoban_utils/control/control.h"

namespace rhoban_utils
{
Control::Control()
{
  k_p = 1.0;
  k_i = 0.0;

  acc = 0;
  min = -100.0;
  max = 100.0;
}

void Control::reset()
{
  acc = 0;
}

float Control::update(float error, float dt)
{
  // P gain
  float tmp = k_p * error;

  // I gain
  acc += k_i * error * dt;
  if (acc > max)
    acc = max;
  if (acc < min)
    acc = min;
  tmp += acc;

  // Bounding
  if (tmp < min)
    tmp = min;
  if (tmp > max)
    tmp = max;

  output = tmp;
  return output;
}

}  // namespace rhoban_utils
