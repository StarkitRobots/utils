#pragma once

namespace starkit_utils
{
class VariationBound
{
public:
  static void update(double& output, double target, double limit, double dt);
  static void update(float& output, float target, float limit, float dt);
};

}  // namespace starkit_utils
