#pragma once

#include <chrono>
#include <string>

namespace starkit_utils
{
class TimeStamp : public std::chrono::time_point<std::chrono::steady_clock>
{
public:
  TimeStamp();
  TimeStamp(const std::chrono::time_point<std::chrono::steady_clock>& timePoint);
  static TimeStamp now();

  static TimeStamp fromMS(unsigned long msSinceEpoch);

  double getTimeSec() const;
  double getTimeMS() const;
};

/// Uses system_clock to extract a formatted time: format is:
/// - YYYY_MM_DD_HHhMMmSSs Ex: 2016_03_25_35h23m12s
/// Funtion is reentrant
std::string getFormattedTime();

/**
 * Return the offset from steady_clock to system_clock in us:
 * steady_clock + offset = system_clock
 */
int64_t getSteadyClockOffset();

}  // namespace starkit_utils

double diffSec(const starkit_utils::TimeStamp& src, const starkit_utils::TimeStamp& dst);
double diffMs(const starkit_utils::TimeStamp& src, const starkit_utils::TimeStamp& dst);

bool operator<(const starkit_utils::TimeStamp& ts1, const starkit_utils::TimeStamp& ts2);
bool operator>(const starkit_utils::TimeStamp& ts1, const starkit_utils::TimeStamp& ts2);
