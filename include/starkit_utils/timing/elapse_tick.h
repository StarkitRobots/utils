#pragma once

#include "starkit_utils/timing/time_stamp.h"

namespace starkit_utils
{
/**
 * ElapseTick
 *
 * Implement ticked behaviour
 * with stats and elapsed
 * time computation.
 */
class ElapseTick
{
public:
  /**
   * Initialization
   */
  ElapseTick();

  /**
   * Virtual destructor
   */
  virtual ~ElapseTick();

  /**
   * Run one tick
   */
  void tick();

  /**
   * Timing stats
   */
  bool hasStats;
  double minTime;
  double avgTime;
  double maxTime;

protected:
  /**
   * Ticked implementation
   * Elapsed is the time between last ticj call
   * (in seconds)
   * Return true if the task has really been ticked.
   */
  virtual bool tick(double elapsed) = 0;

private:
  /**
   * Elapsed computation
   */
  TimeStamp lastTimestamp;
};

}  // namespace starkit_utils
