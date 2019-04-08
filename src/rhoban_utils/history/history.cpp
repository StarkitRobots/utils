#include <rhoban_utils/angle.h>
#include "rhoban_utils/history/history.h"
#include "rhoban_utils/util.h"

namespace rhoban_utils
{
HistoryDouble::HistoryDouble(double window) : History(window)
{
}

HistoryDouble::TimedValue HistoryDouble::readValueFromStream(std::istream& is, bool binary)
{
  TimedValue value;

  if (binary)
  {
      is.read((char*)&value.first, sizeof(double));
      is.read((char*)&value.second, sizeof(double));
  }
  else
  {
    is >> value.first >> value.second;
  }

  return value;
}

void HistoryDouble::writeValueToStream(const TimedValue& value, std::ostream& os, bool binary)
{
  if (binary)
  {
    os.write((const char*)&(value.first), sizeof(double));
    os.write((const char*)&(value.second), sizeof(double));
  }
  else
  {
    os << std::setprecision(15) << value.first << " " << std::setprecision(15) << value.second << std::endl;
  }
}

HistoryAngle::HistoryAngle(double window) : HistoryDouble(window)
{
}

double HistoryAngle::doInterpolate(double valLow, double wLow, double valUp, double wUp) const
{
  Angle angleLow(rad2deg(valLow));
  Angle angleUp(rad2deg(valUp));
  auto result = Angle::weightedAverage(angleLow, wLow, angleUp, wUp);

  return deg2rad(result.getSignedValue());
}
}  // namespace rhoban_utils
