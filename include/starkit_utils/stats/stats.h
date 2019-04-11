#pragma once

#include <vector>

namespace starkit_utils
{
double average(std::vector<double> values);

double variance(std::vector<double> values, double* avg = nullptr);

double standardDeviation(std::vector<double> values, double* avg = nullptr);

}  // namespace starkit_utils
