/*************************************************
 * Publicly released by Starkit System, August 2012
 *             www.starkit-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#pragma once

#include "starkit_utils/threading/mutex.h"

namespace starkit_utils
{
class Condition : public Mutex
{
public:
  Condition();
  virtual ~Condition();

  // wait for the condition to be brodcasted (optional timeout in ms)
  // the condition should be locked before use
  virtual int wait(unsigned int timeout =
#ifndef MSVC
                       0
#else
                       INFINITE
#endif
  );

  // wait for the condition with a particular Mutex
  virtual int wait(Mutex* mutex, unsigned int timeout =
#ifndef MSVC
                                     0
#else
                                     INFINITE
#endif
  );

  // broadcast a condition
  // typically, the condition is locked by the calling thread before being broadcasted and unlocked afterwards
  virtual void broadcast();

private:
#ifndef MSVC
  pthread_cond_t condition;
#else
  CONDITION_VARIABLE condition;
#endif
};

}  // namespace starkit_utils
