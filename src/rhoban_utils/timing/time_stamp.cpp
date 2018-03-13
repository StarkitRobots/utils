#include "rhoban_utils/timing/time_stamp.h"

#include <ctime>

using namespace std::chrono;

namespace rhoban_utils {

TimeStamp::TimeStamp(){}
TimeStamp::TimeStamp(const time_point<steady_clock> & timePoint)
    : time_point<steady_clock>(timePoint) {}

TimeStamp TimeStamp::now()
{
    return TimeStamp(steady_clock::now());
}

TimeStamp TimeStamp::fromMS(unsigned long msSinceEpoch)
{
    return TimeStamp(time_point<steady_clock>(
        milliseconds(msSinceEpoch)));
}

double TimeStamp::getTimeSec() const
{
  return getTimeMS() / 1000;
}

double TimeStamp::getTimeMS() const
{
  return duration_cast<std::chrono::duration<double, std::milli>>(
      time_since_epoch()).count();
}

std::string getFormattedTime() {
 system_clock::time_point now = system_clock::now();
 time_t tt = system_clock::to_time_t(now);
 struct tm tm;
 localtime_r(&tt, &tm);
 char buffer[80];// Buffer is big enough
 sprintf(buffer,"%4d_%2d_%2d_%2dh%2dm%2ds",
         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
         tm.tm_hour, tm.tm_min, tm.tm_sec);
 return std::string(buffer);
}

}


double diffSec(const rhoban_utils::TimeStamp & src,
    const rhoban_utils::TimeStamp & dst)
{
    double elapsedTicks = (dst - src).count();
    return elapsedTicks * steady_clock::period::num / steady_clock::period::den;
}

double diffMs(const rhoban_utils::TimeStamp & src,
    const rhoban_utils::TimeStamp & dst)
{
    return diffSec(src, dst) * 1000;
}

bool operator<(const rhoban_utils::TimeStamp & ts1,
    const rhoban_utils::TimeStamp & ts2)
{
    return diffMs(ts1, ts2) > 0;
}
bool operator>(const rhoban_utils::TimeStamp & ts1,
    const rhoban_utils::TimeStamp & ts2)
{
    return diffMs(ts1, ts2) < 0;
}
