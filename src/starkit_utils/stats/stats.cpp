#include <cmath>
#include <starkit_utils/stats/stats.h>

namespace starkit_utils
{
double average(std::vector<double> values)
{
  if (values.size())
  {
    double result = 0;

    for (auto& v : values)
    {
      result += v;
    }

    return result / values.size();
  }
  else
  {
    return 0;
  }
}

double variance(std::vector<double> values, double* avg_)
{
  double avg = average(values);

  if (avg_ != nullptr)
  {
    *avg_ = avg;
  }

  if (values.size())
  {
    double result = 0;

    for (auto& v : values)
    {
      result += pow(v - avg, 2);
    }

    return result / values.size();
  }
  else
  {
    return 0;
  }
}

double standardDeviation(std::vector<double> values, double* avg)
{
  return sqrt(variance(values, avg));
}
}  // namespace starkit_utils
