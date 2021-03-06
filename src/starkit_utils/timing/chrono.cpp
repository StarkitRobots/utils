/*************************************************
 * Publicly released by Starkit System, August 2012
 *             www.starkit-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/*****************************************************************************/
/*! \file    Starkit::chrono.c
 *  \author  Olivier Ly
 *  \date    2009-10
 *
 *  \brief   Basic tools for time measure.
 *****************************************************************************/

#include <stdlib.h>

#include "starkit_utils/timing/chrono.h"

#ifdef MSVC
#include <sys/timeb.h>
#include <time.h>
#endif

namespace starkit_utils
{
/*!\brief compute tv0-tv1 and put it into dtv */
void compute_tv_diff(const t_chrono* tv0, const t_chrono* tv1, t_chrono* dtv)
{
  if (tv0->tv_sec == tv1->tv_sec)
  {
    dtv->tv_sec = 0;
    dtv->tv_usec = tv0->tv_usec - tv1->tv_usec;
  }
  else
  {
    long int usec_diff;
    dtv->tv_sec = tv0->tv_sec - tv1->tv_sec - 1;
    usec_diff = tv0->tv_usec + (1000000 - tv1->tv_usec);
    if (usec_diff > 1000000)
    {
      dtv->tv_sec = dtv->tv_sec + 1;
      dtv->tv_usec = usec_diff - 1000000;
    }
    else
      dtv->tv_usec = usec_diff;
  }
}

#ifdef MSVC
int gettimeofday(t_chrono* tp, void* tz)
{
  struct _timeb timebuffer;
  _ftime64_s(&timebuffer);
  tp->tv_sec = timebuffer.time;
  tp->tv_usec = timebuffer.millitm * 1000;
  return 0;
}

#endif

t_chrono* chrono_create()
{
  t_chrono* chr = (t_chrono*)malloc(sizeof(t_chrono));
  chrono_reset(chr);
  return chr;
}

void chrono_reset(t_chrono* chr)
{
  gettimeofday(chr, NULL);
}

void delete_chrono(t_chrono* chr)
{
  free(chr);
}

long int chrono_sec(const t_chrono* chr)
{
  t_chrono tv, tv_diff;
  gettimeofday(&tv, NULL);
  compute_tv_diff(&tv, chr, &tv_diff);
  return tv_diff.tv_sec;
}

long int chrono_msec(const t_chrono* chr)
{
  t_chrono tv, tv_diff;
  gettimeofday(&tv, NULL);
  compute_tv_diff(&tv, chr, &tv_diff);
  return tv_diff.tv_sec * 1000 + tv_diff.tv_usec / 1000;
}

long int chrono_usec(const t_chrono* chr)
{
  t_chrono tv, tv_diff;
  gettimeofday(&tv, NULL);
  compute_tv_diff(&tv, chr, &tv_diff);
  return tv_diff.tv_sec * 1000000 + tv_diff.tv_usec;
}

/*****************************************************************************/
/*****************************************************************************/

Chrono::Chrono()
{
  reset();
}

double Chrono::getTime() const
{
  return chrono_usec(&chr) * 0.000001;
}

long Chrono::getTimeUsec() const
{
  return chrono_usec(&chr);
}

long Chrono::getTimeMsec() const
{
  return chrono_msec(&chr);
}

void Chrono::reset()
{
  chrono_reset(&chr);
}

}  // namespace starkit_utils
