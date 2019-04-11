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

#ifndef MSVC
#include <pthread.h>
#else
#include <winsock2.h>
#include <stdio.h>
#endif

//#define DEBUG_MUTEXES

namespace starkit_utils
{
class Condition;

class Mutex
{
  friend class Condition;

public:
  Mutex(void);
  virtual ~Mutex(void);
  virtual void lock(void);
  virtual void unlock(void);

protected:
#ifndef MSVC
  pthread_mutex_t _mutex;
#else
  CRITICAL_SECTION _mutex;
#endif
};
}  // namespace starkit_utils
